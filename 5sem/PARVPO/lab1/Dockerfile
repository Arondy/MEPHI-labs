FROM alpine

RUN apk update && apk add g++ git

WORKDIR /app

RUN git clone https://github.com/Arondy/PARVPO_lab1 .

RUN g++ -o lab1 -fopenmp src/main.cpp

CMD ./lab1 > /output/output.txt
