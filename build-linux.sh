#!/usr/bin/env bash
# Build the NYVERXS .deb on Linux/WSL2 without a Mac.
#
#   wsl --install -d Ubuntu-22.04
#   cd /mnt/c/Users/<you>/Downloads/src\ ios/src\ ios/nyverxs/nyverxs/nyverxs
#   bash build-linux.sh
#
# Output: packages/com.nologs.nyverxs_*_iphoneos-arm64.deb
set -euo pipefail

THEOS="${THEOS:-$HOME/theos}"
SDK_REPO="$THEOS/sdks"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if ! command -v apt-get >/dev/null 2>&1; then
    echo "apt-get not found: this script targets Debian/Ubuntu (WSL2)." >&2
    exit 1
fi

echo "[1/5] system packages"
sudo apt-get update -qq
sudo apt-get install -y -qq \
    build-essential fakeroot git curl xz-utils ca-certificates \
    libssl-dev libplist-dev libtool autoconf automake pkg-config \
    libc6-dev libncurses-dev >/dev/null
# Not in every Ubuntu release; Theos still links without it for this tweak.
sudo apt-get install -y -qq libtapi-dev >/dev/null 2>&1 || true

echo "[2/5] ldid"
if ! command -v ldid >/dev/null 2>&1; then
    case "$(uname -m)" in
        x86_64)  LDID_ASSET="ldid_linux_x86_64" ;;
        aarch64) LDID_ASSET="ldid_linux_aarch64" ;;
        *) echo "unsupported arch $(uname -m)" >&2; exit 1 ;;
    esac
    sudo curl -fsSL -o /usr/local/bin/ldid \
        "https://github.com/ProcursusTeam/ldid/releases/latest/download/$LDID_ASSET"
    sudo chmod +x /usr/local/bin/ldid
fi
ldid -h >/dev/null 2>&1 || true

echo "[3/5] Theos at $THEOS"
if [ ! -f "$THEOS/makefiles/common.mk" ]; then
    git clone --recursive https://github.com/theos/theos.git "$THEOS"
fi

echo "[4/5] iOS SDK"
if [ ! -d "$SDK_REPO" ] || [ -z "$(ls -A "$SDK_REPO" 2>/dev/null | grep -E '^iPhoneOS.*\.sdk$' || true)" ]; then
    rm -rf "$SDK_REPO"
    git clone --filter=blob:none --sparse --depth 1 https://github.com/theos/sdks.git "$SDK_REPO"
    cd "$SDK_REPO"
    SDK="iPhoneOS$(git ls-tree --name-only HEAD | grep -E '^iPhoneOS[0-9.]+\.sdk$' | sed 's/^iPhoneOS//; s/\.sdk$//' | sort -n | tail -1).sdk"
    echo "      using $SDK"
    git sparse-checkout set --cone "$SDK"
    cd "$HERE"
fi

echo "[5/5] build"
export THEOS
make -C "$HERE" clean package FINALPACKAGE=1

echo
ls -la "$HERE/packages/"*.deb
echo "Install on a rootless jailbreak: open the .deb in Sileo/Zebra, or"
echo "  dpkg -i <file>.deb   (as root on the device)"
