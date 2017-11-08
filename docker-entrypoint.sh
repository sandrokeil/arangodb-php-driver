#!/bin/bash

make --directory=/app/build install

exec $@
