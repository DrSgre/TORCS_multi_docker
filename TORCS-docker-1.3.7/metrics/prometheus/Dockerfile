FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \ 
    wget \ 
    tar \ 
    sudo

RUN wget https://github.com/prometheus/prometheus/releases/download/v2.0.0/prometheus-2.0.0.linux-amd64.tar.gz -O /tmp/prometheus-2.0.0.linux-amd64.tar.gz

RUN tar -xvzf /tmp/prometheus-2.0.0.linux-amd64.tar.gz --directory /tmp/ --strip-components=1

RUN /tmp/prometheus --version

RUN rm /tmp/prometheus.yml

RUN touch /tmp/prometheus.yml

RUN echo "global:" >> /tmp/prometheus.yml
RUN echo "  scrape_interval: 10s" >> /tmp/prometheus.yml
RUN echo "scrape_configs:" >> /tmp/prometheus.yml
RUN echo "  - job_name: test-etcd" >> /tmp/prometheus.yml
RUN echo "    static_configs:" >> /tmp/prometheus.yml
RUN echo "    - targets: ['etcd:2379']" >> /tmp/prometheus.yml

RUN cat /tmp/prometheus.yml

RUN nohup /tmp/prometheus \
    --config.file /tmp/prometheus.yml \
    --web.listen-address ":9090" \
    --storage.local.path "test-etcd.data" >> /tmp/test-etcd.log  2>&1 &
    
WORKDIR /tmp/

CMD ./prometheus
