/**
 * @brief Redis-like in-memory server with UCall and ustore.
 */
#include <cstdio> // `std::printf`

#include "ucall/ucall.h"
#include "ustore/cpp/db.hpp"

using namespace unum::ustore;
using namespace unum;

static ustore_database_t db{};
static ustore_arena_t arena{};

static void set(ucall_call_t call, ucall_callback_tag_t)
{
    int64_t *key_ptr{};
    ucall_str_t value_ptr{};
    size_t value_len{};

    bool key_found = ucall_param_named_i64(call, "key", 0, key_ptr);
    bool value_found = ucall_param_named_str(call, "value", 5, &value_ptr, &value_len);
    if (!key_found || !value_found)
        return ucall_call_reply_error_invalid_params(call);

    status_t status;
    ustore_write_t write{};
    write.db = db;
    write.error = status.member_ptr();
    write.arena = &arena;
    write.tasks_count = 1;
    write.keys = reinterpret_cast<ustore_key_t *>(key_ptr);
    write.lengths = reinterpret_cast<ustore_length_t *>(&value_len);
    write.values = reinterpret_cast<ustore_bytes_cptr_t *>(&value_ptr);

    ustore_write(&write);
    if (status)
        return ucall_call_reply_content(call, "OK", 2);
    return ucall_call_reply_content(call, "", 0);
}

static void get(ucall_call_t call, ucall_callback_tag_t)
{
    int64_t *key_ptr{};
    bool key_found = ucall_param_named_i64(call, "key", 0, key_ptr);
    if (!key_found)
        return ucall_call_reply_error_invalid_params(call);

    status_t status;
    ustore_length_t *found_lengths{};
    ustore_bytes_ptr_t found_values{};

    ustore_read_t read{};
    read.db = db;
    read.error = status.member_ptr();
    read.arena = &arena;
    read.tasks_count = 1;
    read.keys = reinterpret_cast<ustore_key_t *>(key_ptr);
    read.lengths = &found_lengths;
    read.values = &found_values;

    ustore_read(&read);
    if (status)
        return ucall_call_reply_content(call, reinterpret_cast<ucall_str_t>(*found_values), *found_lengths);
    return ucall_call_reply_content(call, "", 0);
}

int main(int argc, char **argv)
{

    status_t status;
    ustore_database_init_t database_init{};
    database_init.db = &db;
    database_init.error = status.member_ptr();
    ustore_database_init(&database_init);
    if (!status)
    {
        std::printf("Failed To Open DataBase!\n");
        return -1;
    }

    ucall_server_t server{};
    ucall_config_t config{};
    config.port = 6378;
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