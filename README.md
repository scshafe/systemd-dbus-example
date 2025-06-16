
A small working example to demonstrate functionality for:
- systemd
- dbus communication
- logging to journald

### Building

make targets include `service` for the systemd daemon "scshafe-test-service" and `client` for a small client that can be used to send messages to the daemon.

```
$ make service
$ make client
$ make install
```

Installing will put "scshafe-test-service" into `/usr/bin` and "scshafe-test-service.service" into `/etc/systemd/system`. From there, run:

```
## Reload the systemd unit files
$ sudo systemctl reload-daemon

$ sudo systemctl start scshafe-test-service

## View the logs in a terminal
$ sudo journalctl -f | grep scshafe

## From a separate terminal, send a message with the client
$ ./scshafe-test-client
```
