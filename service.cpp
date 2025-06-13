#include <iostream>

// For DBus
#include <dbus/dbus.h>

// For printing to journalctl
#include <systemd/sd-journal.h>



int main() {
    DBusError err;
    dbus_error_init(&err);

    DBusConnection* conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        sd_journal_print(LOG_ERR, "Error connecting to bus:");
        //sd_journal_print(LOG_ERR, "Error connecting to bus:" + err.message);
        //std::cerr << "Error connecting to bus: " << err.message << std::endl;
        dbus_error_free(&err);
        return 1;
    }

    int ret = dbus_bus_request_name(conn, "scshafe.example.MyService", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
    if (dbus_error_is_set(&err)) {
        sd_journal_print(LOG_ERR, "Error requesting name: ");
        //sd_journal_print(LOG_ERR, "Error requesting name: " + err.message);
        //std::cerr << "Error requesting name: " << err.message << std::endl;
        dbus_error_free(&err);
        dbus_connection_unref(conn);
        return 1;
    }
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        sd_journal_print(LOG_ERR, "Name not available.");
        //std::cerr << "Name not available." << std::endl;
        dbus_connection_unref(conn);
        return 1;
    }

    sd_journal_print(LOG_INFO, "service started.");

    while (true) {
        dbus_connection_read_write_dispatch(conn, -1);
        DBusMessage* msg = dbus_connection_pop_message(conn);
        if (msg == nullptr) continue;

        sd_journal_print(LOG_INFO, "message received.");

        if (dbus_message_is_method_call(msg, "scshafe.example.MyInterface", "MyMethod")) {
            DBusMessage* reply = dbus_message_new_method_return(msg);
            dbus_connection_send(conn, reply, nullptr);
            dbus_message_unref(reply);
            sd_journal_print(LOG_INFO, "method called.");
        }

        dbus_message_unref(msg);
    }

    dbus_connection_unref(conn);
    return 0;
}
