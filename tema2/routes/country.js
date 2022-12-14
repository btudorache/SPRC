const express = require('express');
const { body, param, validationResult } = require('express-validator');

const countryRoutes = express.Router()

const dbo = require('../db/connection');

countryRoutes.route('/').get(
    async (req, res) => {
        const dbConnect = dbo.getDb();

        dbConnect
            .collection('countries')
            .find()
            .toArray((err, result) => {
                if (err) {
                    res.status(400).json({
                        error: 'Failed to fetch countries'
                    });
                } else {
                    res.status(200).json(result.map(country => {
                        return {
                            id: country._id,
                            nume: country.nume_tara,
                            lat: country.latitudine,
                            lon: country.longitudine
                        }
                    }));
                }
            });
    }
);

countryRoutes.route('/').post(
    body('nume').isString().not().isEmpty(), 
    body('lat').isFloat().not().isEmpty(),
    body('lon').isFloat().not().isEmpty(),
    body().custom(body => {
        const keys = ['nume', 'lat', 'lon'];
        return Object.keys(body).every(key => keys.includes(key));
    }).withMessage('Some extra parameters are sent'),
    async (req, res) => {
        const errors = validationResult(req);
        if (!errors.isEmpty()) {
            return res.status(400).json({ errors: errors.array() })
        }

        const countryId = await dbo.getNextSequence('countryId');
        const countryModel = {
            _id: countryId,
            nume_tara: req.body.nume,
            latitudine: req.body.lat,
            longitudine: req.body.lon
        }

        const dbConnect = dbo.getDb();
        dbConnect
            .collection('countries')
            .insertOne(countryModel, (err, result) => {
                if (err) {
                    res.status(400).send({ error: 'Failed to insert country '});
                } else {
                    res.status(201).send({ id: result.insertedId });
                }
            });
    }
);

countryRoutes.route('/:id?').put(
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
        const filter = { _id: parseInt(req.params.id) };
        const updates = {
            $set: {
                nume_tara: req.body.nume,
                latitudine: req.body.lat,
                longitudine: req.body.lon
            }
        }

        dbConnect
            .collection('countries')
            .updateOne(filter, updates, (err, result) => {
                if (err) {
                    res.status(400).json({ error: err });
                } else if (result.modifiedCount === 0) {
                    res.status(404).json({ error: 'Country not found' });
                } else {
                    res.status(200).send();
                }
            })
    }
);

countryRoutes.route('/:id?').delete(
    param('id').isInt().not().isEmpty(),
    async (req, res) => {
        const dbConnect = dbo.getDb();
        const filter = { _id: parseInt(req.params.id) };

        dbConnect
            .collection('countries')
            .deleteOne(filter, (err, result) => {
                if (err) {
                    res.status(400).send({ error: err });
                } else if (result.deletedCount === 0) {
                    res.status(404).json({ error: 'Country not found' })
                } 
                else {
                    res.status(200).send();
                }
            });
    }
);

module.exports = countryRoutes;