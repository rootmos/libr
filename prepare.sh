#!/bin/bash

set -o nounset -o pipefail -o errexit

SUDO=${SUDO-}
DISTRO=${DISTRO-}
UPDATE=${UPDATE-}
while getopts "ud:sS:-" OPT; do
    case $OPT in
        d) DISTRO=$OPTARG ;;
        u) UPDATE=1 ;;
        s) SUDO=sudo ;;
        S) SUDO=$OPTARG ;;
        -) break ;;
        ?) usage 2 ;;
    esac
done
shift $((OPTIND-1))

if [ -z "$DISTRO" ]; then
    if command -v lsb_release >/dev/null; then
        DISTRO=$(lsb_release -is)
    elif command -v pacman >/dev/null; then
        DISTRO="Arch"
    elif command -v apt-get >/dev/null; then
        # TODO: debian
        DISTRO="Ubuntu"
    else
        echo "unable to figure out distribution: $DISTRO" 1>&2
        exit 1
    fi
fi
echo "distro: $DISTRO" 1>&2

if [ "$DISTRO" = "Arch" ] || command -v pacman >/dev/null; then
    if [ -n "$UPDATE" ]; then
        $SUDO pacman -Sy 1>&2
    fi

    PKGs=(git)
    PKGs+=(make gcc pkgconf)
    PKGs+=(python)
    PKGs+=(lua)
    PKGs+=(gsl)

    $SUDO pacman -S --noconfirm "${PKGs[@]}" 1>&2
elif [ "$DISTRO" = "Ubuntu" ] || command -v apt-get >/dev/null; then
    if [ -n "$UPDATE" ]; then
        $SUDO apt-get update 1>&2
    fi

    PKGs=(git ca-certificates)
    PKGs+=(make gcc pkg-config)
    PKGs+=(python3 python3-toml)
    PKGs+=(liblua5.4-dev libgsl-dev)

    $SUDO apt-get install --yes \
        --no-install-recommends --no-install-suggests \
        1>&2  "${PKGs[@]}"
else
    echo "unconfigured distribution: $DISTRO" 1>&2
    exit 1
fi
