db.createUser(
    {
      user: "user12345",
      pwd: "pass12345",
      roles: [ { role: "readWrite", db: "geography" } ]
    }
);
