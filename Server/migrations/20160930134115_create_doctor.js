
exports.up = function(knex, Promise) {
  return Promise.all([
    knex.schema.createTableIfNotExists('doctors', function(table) {
      table.increments('id');
      table.timestamp('created_at').defaultTo(knex.fn.now());
      table.integer('user_id');
      table.integer('practice_id');
      table.string('firstName');
      table.string('lastName');
    })
  ])
};

exports.down = function(knex, Promise) {
  return Promise.all([
    knex.schema.dropTable('doctors')
  ])
};
