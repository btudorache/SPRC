require('dotenv').config();

const express = require('express')
const dbo = require('./db/connection');

const app = express()
const PORT = process.env.NODE_DOCKER_PORT || 8080;

app.use(express.json());
app.use('/api/countries', require('./routes/country'));
app.use('/api/cities', require('./routes/city'));
app.use('/api/temperatures', require('./routes/temperature'));

dbo.connectToServer().then(() => {
    app.listen(PORT, () => {
        console.log(`Server is running on port: ${PORT}`);
    });
});
