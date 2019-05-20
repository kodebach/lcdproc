# The yard2 driver

## General

The yard2 driver handles the yard2 LCD device. The driver functionality
is currently limited to character LCD.

This driver writes via socket to yard2srvd deamon.

The yard2 driver assumes the character encoding ISO 8859-1.

## Configuration

The only configuration parameter is Size. All other settings are done in
yard2srvd daemon.
