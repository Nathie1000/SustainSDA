var User = require('../models/model');

exports.seed = function(knex, Promise) {
  //creates an admin account with
  //username: admin
  //password: 12345678
      return Promise.all([
          knex('users').insert({id: 1, password: 'M+N0vKD/F/VrOSsU9p15Yw8+hFZRg8IKfNDNf/QXEdjboUaZTY06rpiF5yQm7Ufb+10nYG5tNwowShyxTkBeyeH2P6P93Uy7IaxOdu/tfaYzv3ebNHUYlvjde6W2nARa4iiRb0H2+W3s/5+XT4GZ6yeWUwgacGRMfj7tJKiE4QQ=', salt: 'pcQXX60On6mdds2UsdDmPSFn+OlYtNa+MhKxkobg8u9y3n+POddaJUzyR5kjV5TZmVLfKEpyr0n6h6mWN/0Vpurs7D6/f0nC+rw43PgQYLWTlIZoBRtaeoPJz7x2IcJAOpHHw/4sNRUGurDT/041bl2Oc4FYO2qgGEbTiFhuXo0=', username: 'S19OhexQi6gQg/jUSg2XBA==', role: 2})
      ]);
};
