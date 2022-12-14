FROM node:14

WORKDIR /bezkoder-app
COPY package.json .
RUN npm install --omit=dev
COPY . .
CMD npm start