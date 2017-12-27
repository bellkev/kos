#! /bin/bash

rm *.log || true # These can make for a massive build context
docker build -t bellkev/kos .
