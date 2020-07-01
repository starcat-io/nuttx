#G!/bin/bash
# tools/check-nuttx-release.sh
#
#   Copyright (C) 2020 Adam Feuer. All rights reserved.
#   Author: Adam Feuer <adam@starcat.io>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

set -e

BASE_URL="https://dist.apache.org/repos/dist/dev/incubator/nuttx"
TEMPDIR="dist.apache.org"
ORIGINAL_DIR="$(pwd)"
trap "rm -rf $TEMPDIR" EXIT

function download_release() {
    rm -rf "$TEMPDIR"
    wget -r -np -R "index.html*" -P . --cut-dirs 7 "$URL"
    cd "$TEMPDIR"
}

function check_sha512() {
    # check release sha512
    RELEASE_FILE=$1
    echo "Checking $RELEASE_FILE sha512..."
    sha512sum -c "$RELEASE_FILE.sha512"
}

function check_gpg() {
    # check nuttx sha512 and gpg
    RELEASE_FILE=$1
    echo "Checking $RELEASE_FILE GPG signature:"
    gpg --verify "$RELEASE_FILE.asc" "$RELEASE_FILE"
    echo
}

function check_required_files() {
    # check nuttx for required files
    RELEASE_FILE=$1
    RELEASE_DIR=$2 
    rm -rf "$RELEASE_DIR"
    tar xf "$RELEASE_FILE"
    ERROR=0
    if [ ! -f "$RELEASE_DIR/LICENSE" ]; then
        echo "LICENSE file not present."
        ERROR=1
    fi
    if [ ! -f "$RELEASE_DIR/NOTICE" ]; then
        echo "NOTICE file not present."
        ERROR=1
    fi
    if [ ! -f "$RELEASE_DIR/README.txt" ]; then
        echo "README.txt file not present."
        ERROR=1
    fi
    if [ ! -f "$RELEASE_DIR/DISCLAIMER-WIP" ]; then
        echo "DISCLAIMER-WIP file not present."
        ERROR=1
    fi
    if [ 0 -eq $ERROR ]; then
        echo "OK: All required files exist."
    fi
}

function check_nuttx() {
    # check nuttx sha512 and gpg
    RELEASE_FILE="$(ls *.tar.gz|head -1)"
    check_sha512 "$RELEASE_FILE" 
    check_gpg "$RELEASE_FILE"
    check_required_files "$RELEASE_FILE" "nuttx"
}

function check_nuttx_apps() {
    # check nuttx-apps sha512 and gpg
    RELEASE_FILE="$(ls *.tar.gz|head -2| tail -1)"
    check_sha512 "$RELEASE_FILE"
    check_gpg "$RELEASE_FILE"
    check_required_files "$RELEASE_FILE" "apps"
}

function usage() {
    echo "Usage: $0 <URL-of-release-candidate-directory-or-release-name>"
    echo "   Downloads directory listing from the URL given, or the release name,"
    echo "   finds all file URLS in that page, downloads nuttx and nuttx-apps sha512,"
    echo "   asc, and tar.gz files, checks their SHA512 and GPG signatures, and "
    echo "   checks the unpacked directories for required files. Creates a temporary "
    echo "   directory to do its work in." 
    echo 
}

if [ "-h" == "$1" ]; then
    usage
    exit 0
fi

if [ -z "$1" ]; then
    usage
    exit 0
fi

ARG=$1
if [[ "$ARG" =~ ^"http".* ]]; then
  URL="$1/"
else
  URL="$BASE_URL/$1/"
fi

download_release
check_nuttx 
check_nuttx_apps 
cd "$ORIGINAL_DIR"
