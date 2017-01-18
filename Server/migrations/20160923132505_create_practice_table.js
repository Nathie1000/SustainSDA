
exports.up = function(knex, Promise) {
  return Promise.all([
    knex.schema.createTableIfNotExists('practices', function(table) {
      table.increments('id');
      table.timestamp('created_at').defaultTo(knex.fn.now());
      table.string('name').unique();
      table.string('city');
    })
  ])
}

exports.down = function(knex, Promise) {
  return Promise.all([
    knex.schema.dropTable('practices')
  ])
}
