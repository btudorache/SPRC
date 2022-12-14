const { MongoClient } = require("mongodb");

const {
    DB_USER,
    DB_PASSWORD,
    DB_HOST,
    DB_PORT,
    DB_NAME
} = process.env;
const connectionString = `mongodb://${DB_USER}:${DB_PASSWORD}@${DB_HOST}:${DB_PORT}/?authMechanism=DEFAULT&authSource=${DB_NAME}`;


const client = new MongoClient(connectionString, {
  useNewUrlParser: true,
  useUnifiedTopology: true,
});

let dbConnection;

module.exports = {
    connectToServer: async () => {
        await client.connect();

        dbConnection = client.db("geography");

        const counterNames = ["countryId", "cityId", "temperatureId"];

        counterNames.forEach(name => {
            dbConnection
                .collection('counters')
                .updateOne(
                    { _id: name }, 
                    { $setOnInsert: {
                        _id: name,
                        seq: 1
                    }}, 
                    { upsert: true}
                )
        })

        console.log("Successfully connected to MongoDB.");
    },

    getDb: () => {
        return dbConnection;
    },

    getNextSequence: async (name) => {
        const res = await dbConnection
            .collection('counters')
            .findOneAndUpdate(
                { _id: name },
                { $inc: { seq: 1 } },
                { }
            );

        return res.value.seq;
    }
};