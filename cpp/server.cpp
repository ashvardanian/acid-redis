/**
 * @brief Redis-like in-memory server with UCall and ustore.
 */
#include <cstdio> // `std::printf`

#include "ucall/ucall.h"
#include "ustore/cpp/db.hpp"

using namespace unum::ustore;
using namespace unum;

static database_t db;

static void set(ucall_call_t call, ucall_callback_tag_t)
{
    ustore_key_t *key_ptr{};
    char const *value_ptr{};
    size_t value_len{};

    bool key_found = ucall_param_named_i64(call, "key", 0, reinterpret_cast<int64_t *>(&key_ptr));
    bool value_found = ucall_param_named_str(call, "value", 5, &value_ptr, &value_len);
    if (!key_found || !value_found)
        return ucall_call_reply_error_invalid_params(call);

    auto main = db.main();
    value_view_t value(value_ptr, value_len);
    auto status = main[*key_ptr].assign(value);
    if (status)
        return ucall_call_reply_content(call, "OK", 2);
    return ucall_call_reply_content(call, "", 0);
}

static void get(ucall_call_t call, ucall_callback_tag_t)
{
    ustore_key_t *key_ptr{};
    bool key_found = ucall_param_named_i64(call, "key", 0, reinterpret_cast<int64_t *>(&key_ptr));
    if (!key_found)
        return ucall_call_reply_error_invalid_params(call);

    auto main = db.main();
    auto result = main[*key_ptr].value();

    if (result)
        return ucall_call_reply_content(call, result->c_str(), result->size());
    return ucall_call_reply_content(call, "", 0);
}

int main(int argc, char **argv)
{

    db.open().throw_unhandled();
    ucall_server_t server{};
    ucall_config_t config{};
    config.port = 6379;
    ucall_init(&config, &server);
    if (!server)
    {
        std::printf("Failed To Initialize Server!\n");
        return -1;
    }

    std::printf("Listening on port: %i\n", config.port);
    ucall_add_procedure(server, "set", &set, NULL);
    ucall_add_procedure(server, "get", &get, NULL);

    ucall_take_calls(server, 0);
    ucall_free(server);
    return 0;
}