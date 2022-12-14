const express = require('express');
const { body, param, validationResult } = require('express-validator');
const mongoDb = require('mongodb');

const temperatureRoutes = express.Router()

const dbo = require('../db/connection');

temperatureRoutes.route('/').get(
    async (req, res) => {
        const filters = {}
        if (req.query.lat !== undefined) {
            filters.latitudine = parseFloat(req.query.lat);
        }
        if (req.query.lon !== undefined) {
            filters.longitudine = parseFloat(req.query.lon);
        }
        if (req.query.from !== undefined) {
            filters.timestamp = { $gte: new Date(req.query.from) }
        }
        if (req.query.until !== undefined) {
            filters.timestamp = { $lte: new Date(req.query.until) }
        }

        const dbConnect = dbo.getDb();

        dbConnect
            .collection('temperatures')
            .aggregate([
                {
                    "$lookup": {
                        "from": "cities",
                        "localField": "id_oras",
                        "foreignField": "_id",
                        "as": "cities"
                    }
                },
                {
                    "$unwind": "$cities"
                },
                {
                    "$project": {
                        "_id": 1,
                        "latitudine": "$cities.latitudine",
                        "longitudine": "$cities.longitudine",
                        "timestamp": 1,
                        "valoare": 1
                    }
                },
                {
                    "$match": filters
                }
            ])
            .toArray((err, result) => {
                if (err) {
                    res.status(400).json({
                        error: 'Failed to fetch temperatures'
                    });
                } else {
                    res.status(200).json(result.map(temperature => {
                        return {
                            id: temperature._id,
                            valoare: temperature.valoare,
                            timestamp: temperature.timestamp.toISOString().split('T')[0],
                        }
                    }));
                }
            });
    }
);

temperatureRoutes.route('/cities/:id_oras?').get(
    param('id_oras').isInt().not().isEmpty(),
    async (req, res) => {
        const errors = validationResult(req);
        if (!errors.isEmpty()) {
            return res.status(400).json({ errors: errors.array() })
        }

        const filters = {}
        filters.id_oras = parseInt(req.params.id_oras);
        if (req.query.from !== undefined) {
            filters.timestamp = { $gte: new Date(req.query.from) }
        }
        if (req.query.until !== undefined) {
            filters.timestamp = { $lte: new Date(req.query.until) }
        }

        const dbConnect = dbo.getDb();

        dbConnect
            .collection('temperatures')
            .aggregate([
                {
                    "$lookup": {
                        "from": "cities",
                        "localField": "id_oras",
                        "foreignField": "_id",
                        "as": "cities"
                    }
                },
                {
                    "$unwind": "$cities"
                },
                {
                    "$match": filters
                }
            ])
            .toArray((err, result) => {
                if (err) {
                    res.status(400).json({
                        error: 'Failed to fetch temperatures'
                    });
                } else {
                    res.status(200).json(result.map(temperature => {
                        return {
                            id: temperature._id,
                            valoare: temperature.valoare,
                            timestamp: temperature.timestamp.toISOString().split('T')[0],
                        }
                    }));
                }
            });
    }
);

temperatureRoutes.route('/countries/:id_tara?').get(
    param('id_tara').isInt().not().isEmpty(),
    async (req, res) => {
        const errors = validationResult(req);
        if (!errors.isEmpty()) {
            return res.status(400).json({ errors: errors.array() })
        }

        const filters = {}
        filters.id_tara = parseInt(req.params.id_tara);
        if (req.query.from !== undefined) {
            filters.timestamp = { $gte: new Date(req.query.from) }
        }
        if (req.query.until !== undefined) {
            filters.timestamp = { $lte: new Date(req.query.until) }
        }

        const dbConnect = dbo.getDb();

        dbConnect
            .collection('temperatures')
            .aggregate([
                {
                    "$lookup": {
                        "from": "cities",
                        "localField": "id_oras",
                        "foreignField": "_id",
                        "as": "cities"
                    }
                },
                {
                    "$unwind": "$cities"
                },
                {
                    "$lookup": {
                        "from": "countries",
                        "localField": "cities.id_tara",
                        "foreignField": "_id",
                        "as": "countries"
                    }
                },
                {
                    "$unwind": "$countries"
                },
                {
                    "$project": {
                        "_id": 1,
                        "id_tara": "$countries._id",
                        "timestamp": 1,
                        "valoare": 1
                    }
                },
                {
                    "$match": filters
                }
            ])
            .toArray((err, result) => {
                if (err) {
                    res.status(400).json({
                        error: 'Failed to fetch temperatures'
                    });
                } else {
                    res.status(200).json(result.map(temperature => {
                        return {
                            id: temperature._id,
                            valoare: temperature.valoare,
                            timestamp: temperature.timestamp.toISOString().split('T')[0],
                        }
                    }));
                }
            });
    }
);

temperatureRoutes.route('/').post(
    body('idOras').isInt().not().isEmpty(), 
    body('valoare').isFloat().not().isEmpty(),
    body().custom(body => {
        const keys = ['idOras', 'valoare'];
        return Object.keys(body).every(key => keys.includes(key));
    }).withMessage('Some extra parameters are sent'),
    async (req, res) => {
        const errors = validationResult(req);
        if (!errors.isEmpty()) {
            return res.status(400).json({ errors: errors.array() })
        }

        const dbConnect = dbo.getDb();
        const cityIdPresent = await dbConnect
            .collection('cities')
            .findOne({ _id: req.body.idOras});
        
        if (cityIdPresent === null) {
            return res.status(400).send({ error: 'There is no city with that id_oras'});
        }

        const temperatureId = await dbo.getNextSequence('temperatureId');
        const temperatureModel = {
            _id: temperatureId,
            timestamp: new Date(),
            id_oras: req.body.idOras,
            valoare: req.body.valoare,
        }

        dbConnect
            .collection('temperatures')
            .insertOne(temperatureModel, (err, result) => {
                if (err) {
                    res.status(400).send({ error: 'Failed to insert temperature'});
                } else {
                    res.status(201).send({ id: result.insertedId });
                }
            });
    }
);

temperatureRoutes.route('/:id?').put(
    param('id').isInt().not().isEmpty(),
    body('id').isInt().not().isEmpty(),
    body('valoare').isFloat().not().isEmpty(),
    body().custom(body => {
        const keys = ['id', 'valoare'];
        return Object.keys(body).every(key => keys.includes(key));
    }).withMessage('Some extra parameters are sent'),
    async (req, res) => {
        const errors = validationResult(req);
        if (!errors.isEmpty()) {
            return res.status(400).json({ errors: errors.array() })
        }

        const dbConnect = dbo.getDb();

        // In enunt cerea si idOras, dar in teste nu e folosit 
        //
        // const cityIdPresent = await dbConnect
        //     .collection('cities')
        //     .findOne({ _id: req.body.idOras});
        
        // if (cityIdPresent === null) {
        //     return res.status(400).send({ error: 'There is no city with that idOras'});
        // }

        const filter = { _id: parseInt(req.params.id) };
        const updates = {
            $set: {
                id_oras: req.body.idOras,
                valoare: req.body.valoare
            }
        }

        dbConnect
            .collection('temperatures')
            .updateOne(filter, updates, (err, result) => {
                if (err) {
                    res.status(400).json({ error: err });
                } else if (result.modifiedCount === 0) {
                    res.status(404).json({ error: 'Temperature not found' });
                } else {
                    res.status(200).send();
                }
            })
    }
);

temperatureRoutes.route('/:id?').delete(
    param('id').isInt().not().isEmpty(),
    async (req, res) => {
        const dbConnect = dbo.getDb();
        const filter = { _id: parseInt(req.params.id) };

        dbConnect
            .collection('temperatures')
            .deleteOne(filter, (err, result) => {
                if (err) {
                    res.status(400).send({ error: err });
                } else if (result.deletedCount === 0) {
                    res.status(404).json({ error: 'Temperature not found' })
                } 
                else {
                    res.status(200).send();
                }
            });
    }
);

module.exports = temperatureRoutes;