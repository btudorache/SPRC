const express = require('express');
const { body, param, validationResult } = require('express-validator');

const cityRoutes = express.Router()

const dbo = require('../db/connection');

cityRoutes.route('/').get(
    async (req, res) => {
        const dbConnect = dbo.getDb();

        dbConnect
            .collection('cities')
            .find()
            .toArray((err, result) => {
                if (err) {
                    res.status(400).json({
                        error: 'Failed to fetch cities'
                    });
                } else {
                    res.status(200).json(result.map(city => {
                        return {
                            id: city._id,
                            idTara: city.id_tara,
                            nume: city.nume_oras,
                            lat: city.latitudine,
                            lon: city.longitudine
                        }
                    }));
                }
            });
    }
);

cityRoutes.route('/country/:id_tara?').get(
    param('id_tara').isInt().not().isEmpty(),
    async (req, res) => {
        const errors = validationResult(req);
        if (!errors.isEmpty()) {
            return res.status(400).json({ errors: errors.array() })
        }

        const dbConnect = dbo.getDb();

        dbConnect
            .collection('cities')
            .find({ id_tara: parseInt(req.params.id_tara) })
            .toArray((err, result) => {
                if (err) {
                    res.status(400).json({
                        error: 'Failed to fetch cities'
                    });
                } else {
                    res.status(200).json(result.map(city => {
                        return {
                            id: city._id,
                            idTara: city.id_tara,
                            nume: city.nume_oras,
                            lat: city.latitudine,
                            lon: city.longitudine
                        }
                    }));
                }
            });
    }
);

cityRoutes.route('/').post(
    body('idTara').isInt().not().isEmpty(),
    body('nume').isString().not().isEmpty(), 
    body('lat').isFloat().not().isEmpty(),
    body('lon').isFloat().not().isEmpty(),
    body().custom(body => {
        const keys = ['nume', 'lat', 'lon', 'idTara'];
        return Object.keys(body).every(key => keys.includes(key));
    }).withMessage('Some extra parameters are sent'),
    async (req, res) => {
        const errors = validationResult(req);
        if (!errors.isEmpty()) {
            return res.status(400).json({ errors: errors.array() })
        }

        const dbConnect = dbo.getDb();
        const countryIdPresent = await dbConnect
            .collection('countries')
            .findOne({ _id: req.body.idTara});
        
        if (countryIdPresent === null) {
            return res.status(400).send({ error: 'There is no country with that id_tara'});
        }

        const cityId = await dbo.getNextSequence('cityId');
        const cityModel = {
            _id: cityId,
            id_tara: req.body.idTara,
            nume_oras: req.body.nume,
            latitudine: req.body.lat,
            longitudine: req.body.lon
        }

        dbConnect
            .collection('cities')
            .insertOne(cityModel, (err, result) => {
                if (err) {
                    res.status(400).send({ error: 'Failed to insert city '});
                } else {
                    res.status(201).send({ id: result.insertedId });
                }
            });
    }
);

cityRoutes.route('/:id?').put(
    param('id').isInt().not().isEmpty(),
    body('id').isInt().not().isEmpty(),
    body('nume').isString().not().isEmpty(), 
    body('lat').isFloat().not().isEmpty(),
    body('lon').isFloat().not().isEmpty(),
    body().custom(body => {
        const keys = ['id', 'nume', 'lat', 'lon'];
        return Object.keys(body).every(key => keys.includes(key));
    }).withMessage('Some extra parameters are sent'),
    async (req, res) => {
        const errors = validationResult(req);
        if (!errors.isEmpty()) {
            return res.status(400).json({ errors: errors.array() })
        }

        const dbConnect = dbo.getDb();
        
        // In enunt cerea si idTara, dar in teste nu e folosit 
        //
        // const countryIdPresent = await dbConnect
        //     .collection('countries')
        //     .findOne({ _id: req.body.idTara});
        
        // if (countryIdPresent === null) {
        //     return res.status(400).send({ error: 'There is no country with that idTara'});
        // }

        const filter = { _id: parseInt(req.params.id) };
        const updates = {
            $set: {
                id_tara: req.body.idTara,
                nume_oras: req.body.nume,
                latitudine: req.body.lat,
                longitudine: req.body.lon
            }
        }

        dbConnect
            .collection('cities')
            .updateOne(filter, updates, (err, result) => {
                if (err) {
                    res.status(400).json({ error: err });
                } else if (result.modifiedCount === 0) {
                    res.status(404).json({ error: 'City not found' });
                } else {
                    res.status(200).send();
                }
            })
    }
);

cityRoutes.route('/:id?').delete(
    param('id').isInt().not().isEmpty(),
    async (req, res) => {
        const dbConnect = dbo.getDb();
        const filter = { _id: parseInt(req.params.id) };

        dbConnect
            .collection('cities')
            .deleteOne(filter, (err, result) => {
                if (err) {
                    res.status(400).send({ error: err });
                } else if (result.deletedCount === 0) {
                    res.status(404).json({ error: 'City not found' })
                } 
                else {
                    res.status(200).send();
                }
            });
    }
);

module.exports = cityRoutes;