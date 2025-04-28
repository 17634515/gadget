#!/system/bin/sh
MODDIR=${0%/*}
log -p i -t "riru_config" "service.sh executed!"
chmod 755 "$MODDIR/config"

{
  echo "== service.sh started =="
  date
  echo "Starting config..."
} > "$MODDIR/service.log" 2>&1

"$MODDIR/config" --module-path "$MODDIR" >> "$MODDIR/service.log" 2>&1 &
