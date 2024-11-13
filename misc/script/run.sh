#!/bin/sh
# Copyright 2024 NXP
# NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
# accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
# activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
# comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
# terms, then you may not retain, install, activate or otherwise use the software.
# 

systemctl start nvm_daemon
#make sure the executable eledemo and run.sh are in the same folder
DIR=$(dirname $0)
$DIR/eledemo