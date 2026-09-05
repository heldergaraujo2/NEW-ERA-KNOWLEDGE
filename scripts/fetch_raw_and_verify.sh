#!/bin/sh
# NEW-ERA — fetch_raw_and_verify.sh — baixa arquivo raw@commit do upstream e verifica sha256.
# Uso: ./scripts/fetch_raw_and_verify.sh <path> [expected_sha256] [--keep <dest>] [--commit <sha>]
# Sem token. Upstream: wongddd/muonline (pin 580472e — UPSTREAM_PIN.md).
set -u
UP_REPO="wongddd/muonline"
COMMIT="580472e"
EXPECTED=""
KEEP=""
PATH_ARG=""
N=0
while [ $# -gt 0 ]; do
    case "$1" in
        --keep)   KEEP="$2"; shift 2 ;;
        --commit) COMMIT="$2"; shift 2 ;;
        *)        N=$((N+1))
                  [ $N -eq 1 ] && PATH_ARG="$1"
                  [ $N -eq 2 ] && EXPECTED="$1"
                  shift ;;
    esac
done
[ -z "$PATH_ARG" ] && { echo "uso: $0 <path> [expected_sha256] [--keep <dest>] [--commit <sha>]" >&2; exit 64; }
DEST="${KEEP:-/tmp/newera_fetch_$(basename "$PATH_ARG" | tr '/' '_')}"
URL="https://raw.githubusercontent.com/${UP_REPO}/${COMMIT}/$(printf '%s' "$PATH_ARG" | sed 's/ /%20/g')"
if ! curl -fsSL "$URL" -o "$DEST"; then
    echo "FALHA: download $URL" >&2; rm -f "$DEST"; exit 1
fi
BYTES=$(wc -c < "$DEST" | tr -d ' ')
SHA=$(sha256sum "$DEST" | cut -d' ' -f1)
echo "path:   $PATH_ARG"
echo "commit: $COMMIT"
echo "bytes:  $BYTES"
echo "sha256: $SHA"
echo "dest:   $DEST"
if [ -n "${EXPECTED}" ]; then
    if [ "$SHA" != "$EXPECTED" ]; then
        echo "SHA256 DIVERGENTE (esperado $EXPECTED)" >&2
        [ -z "$KEEP" ] && rm -f "$DEST"
        exit 2
    fi
    echo "sha256 OK (bateu com o esperado)"
fi
[ -z "$KEEP" ] && rm -f "$DEST" && echo "(temporário removido; use --keep <dest> para manter)"
exit 0
