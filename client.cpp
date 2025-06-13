#include <dbus/dbus.h>
#include <iostream>

int main() {
    DBusError err;
    DBusConnection* conn;
    DBusMessage* msg;
    DBusMessageIter args;
    DBusPendingCall* pending;
    int ret;
    dbus_uint32_t serial = 0;

    // Initialize error
    dbus_error_init(&err);

    // Connect to the session bus
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        std::cerr << "Connection Error: " << err.message << std::endl;
        dbus_error_free(&err);
        return 1;
    }

    if (conn == nullptr) {
        std::cerr << "Connection Null" << std::endl;
        return 1;
    }

    // Create a new method call message
    msg = dbus_message_new_method_call("scshafe.example.MyService", // Destination service
                                        "/scshafe/example/MyObject",  // Object path
                                        "scshafe.example.MyInterface", // Interface
                                        "MyMethod"); // Method name

    if (msg == nullptr) {
        std::cerr << "Message Null" << std::endl;
        return 1;
    }

    // Append arguments to the message
    dbus_message_iter_init_append(msg, &args);
    const char* str_arg = "Hello from client";
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &str_arg)) {
        std::cerr << "Out of memory" << std::endl;
        dbus_message_unref(msg);
        dbus_connection_close(conn);
        return 1;
    }

    // Send the message
    ret = dbus_connection_send_with_reply(conn, msg, &pending, -1);
    if (!ret) {
        std::cerr << "Out of Memory" << std::endl;
        dbus_message_unref(msg);
        dbus_connection_close(conn);
        return 1;
    }

    if (pending == nullptr) {
        std::cerr << "Pending Call Null" << std::endl;
        dbus_message_unref(msg);
        dbus_connection_close(conn);
        return 1;
    }

    dbus_connection_flush(conn);
    dbus_message_unref(msg);

    // Block until we get a reply
    dbus_pending_call_block(pending);

    // Get the reply
    msg = dbus_pending_call_steal_reply(pending);
    if (msg == nullptr) {
        std::cerr << "Reply Null" << std::endl;
        dbus_pending_call_unref(pending);
        dbus_connection_close(conn);
        return 1;
    }

    dbus_pending_call_unref(pending);

    // Read the reply
    const char* reply_str;
//    if (dbus_message_iter_init(msg, &args) && dbus_message_iter_get_basic(&args, &reply_str)) {
//        std::cout << "Reply from server: " << reply_str << std::endl;
//    } else {
//        std::cerr << "Reply has no arguments!" << std::endl;
//    }

    dbus_message_unref(msg);
    dbus_connection_close(conn);
    return 0;
}
