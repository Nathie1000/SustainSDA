//MOCHT HET FOUT GAAN, unique is toegevoegd haal dit weg
exports.up = function(knex, Promise) {
  return Promise.all([
    knex.schema.createTableIfNotExists('users', function(table) {
      table.increments('id');
      table.timestamp('created_at').defaultTo(knex.fn.now());
      table.string('username').unique();
      table.string('password');
      table.string('salt');
      table.integer('role');
    })
  ]);
};

exports.down = function(knex, Promise) {
  return Promise.all([
    knex.schema.dropTable('users')
  ]);
};
