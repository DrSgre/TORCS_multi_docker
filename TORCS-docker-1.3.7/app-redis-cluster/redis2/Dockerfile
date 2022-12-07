FROM redis:latest

RUN apt-get update && apt-get install iproute2 iputils-ping -y

COPY redis.conf /usr/local/etc/redis/redis.conf
CMD [ "redis-server", "/usr/local/etc/redis/redis.conf" ]
