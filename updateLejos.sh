#!/bin/bash

echo 'updateLejos.sh <server-url> <lejos-version-0-or-1>'
echo ''
echo 'this script is used for low-level testing. Regular users get the updates for lejos from the official openroberta server'

serverUrl="$1"
lejosVersion="$2"
if [[ "$serverUrl" == '' ]]
then
    echo 'the server URL is missing. Exit 12'
    exit 12
elif [[ "$lejosVersion" == '' ]]
then
    echo 'the lejos version (0 or 1) is missing. Exit 12'
    exit 12
fi

lejosIpAddr='10.0.1.1'

run="scp -oKexAlgorithms=+diffie-hellman-group1-sha1 RobotEV3/updateResources/lejos_${lejosVersion}/EV3Menu.jar root@${lejosIpAddr}:/home/root/lejos/bin/utils"
echo "executing: ${run}"
$run

run="echo ${serverUrl} | ssh -oKexAlgorithms=+diffie-hellman-group1-sha1 root@${lejosIpAddr} \"cat > /home/roberta/serverIP.txt\""
echo "executing: ${run}"
$run

run="ssh -oKexAlgorithms=+diffie-hellman-group1-sha1 root@${lejosIpAddr} mkdir -p /home/roberta/lib"
echo "executing: ${run}"
$run

runtime="RobotEV3/updateResources/lejos_${lejosVersion}/EV3Runtime.jar"
json="RobotEV3/updateResources/lejos_${lejosVersion}/json.jar"
websocket="RobotEV3/updateResources/lejos_${lejosVersion}/Java-WebSocket.jar"
run="scp -oKexAlgorithms=+diffie-hellman-group1-sha1 ${runtime} ${json} ${websocket} root@${lejosIpAddr}:/home/roberta/lib"
echo "executing: ${run}"
$run