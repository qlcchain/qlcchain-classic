FROM ubuntu:16.04 AS builder

ARG NETWORK=live
ENV BOOST_ROOT=/tmp/boost_install

ADD ci /tmp/ci

RUN apt-get update -qq && apt-get install -yqq \
    build-essential \
    cmake \
    g++ \
    wget && \
    /tmp/ci/bootstrap_boost.sh -m \
    rm -rf /var/lib/apt/lists/*

ADD ./ /tmp/src

RUN mkdir /tmp/build && \
    cd /tmp/build && \
    cmake /tmp/src -DBOOST_ROOT=${BOOST_ROOT} -DACTIVE_NETWORK=rai_${NETWORK}_network && \
    make qlc_node && \
    cd .. && \
    echo ${NETWORK} > /etc/qlc-network

FROM ubuntu:16.04
EXPOSE 29734/udp 29735
COPY --from=builder /tmp/build/qlc_node /usr/bin
COPY --from=builder /etc/qlc-network /etc
COPY docker/node/entry.sh /entry.sh
COPY docker/node/config /usr/share/qlcchain/config

RUN chmod +x /entry.sh
CMD ["/bin/bash",  "/entry.sh"]
