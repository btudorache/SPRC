#!/bin/bash
export DOCKER_BUILDKIT=0
export COMPOSE_DOCKER_CLI_BUILD=0

docker service create --name registry --publish 5000:5000 registry:2
docker build -t adapter .
docker tag adapter localhost:5000/adapter
docker push localhost:5000/adapter
docker service create --name myservice localhost:5000/adapter

docker swarm init
docker stack deploy -c stack.yml sprc3