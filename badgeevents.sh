#!/usr/bin/env bash

rvmcurl() {
  local url
  url="https://api.particle.io/v1/devices/${1#/}/${2#/}"
  shift || return # function should fail if we weren't passed at least one argument
  curl -XPOST "${rvm_curl_args[@]}" "$url" "$@"
}

i=1 # for testing purposes

rvm_curl_args=(
  -H 'accept:application/json' 
  -H 'content-type:application/json' 
  -H 'x-sitewhere-tenant:sitewhere1234567890' 
  --user admin:password
)

data=$(jq -n --arg hardwareId "$i" '
{
      "arg": $hardwareId,
      "siteToken": "4e6913db-c8d3-4e45-9436-f0a99b502d3c",
      "specializationToken": "82043707-9e3d-441f-bdcc-33cf0f4f7260"
}')

rvmcurl /api/devices -d "$data"