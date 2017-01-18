
exports.up = function(knex, Promise) {
    return Promise.all([
      knex.schema.createTableIfNotExists('patients', function(table) {
        table.increments('id');
        table.timestamp('created_at').defaultTo(knex.fn.now());
        table.integer('user_id',10);
        table.integer('doctor_id',10);
        table.string('firstName');
        table.string('lastName');
        table.integer('battery',10);
        table.string('emergencyNumber').unique();
        table.string('beltNumber').unique();
        table.string('SHDNumber').unique();
      })
      // knex.schema.table('patients', table=>{
      //   table.foreign('user_id').references('Users.id');
      //   table.foreign('practice_id').references('Practices.id');
      // })
    ])
}

exports.down = function(knex, Promise) {
  return Promise.all([
    knex.schema.dropTable('patients')
  ])
}
