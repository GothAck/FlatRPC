#!/usr/bin/env bash
find . -mindepth 1 -maxdepth 1 -type d  | while read dir; do
  server=$(find $dir -name '*_server' -type f -executable)
  client=$(find $dir -name '*_client' -type f -executable)

  [[ -z "$server" || -z "$client" ]] && continue

  echo "Running $dir as test"
  $server &
  sleep 1
  $client Test || exit 1
  wait
done