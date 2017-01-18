
exports.up = function(knex, Promise) {
  return Promise.all([
    knex.schema.createTableIfNotExists('goals', function(table) {
      table.increments('id');
      table.timestamp('created_at').defaultTo(knex.fn.now());
      table.integer('patient_id');
      table.integer('steps');
      table.date('date');
      table.integer('achieved');
    })
  ])
};

exports.down = function(knex, Promise) {
  return Promise.all([
    knex.schema.dropTable('goals')
  ])
};
