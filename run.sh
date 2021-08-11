#!/bin/sh

ninja && qemu-system-i386 -kernel build/kernel
