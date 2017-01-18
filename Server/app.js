//set DEBUG=sustain-express:* & npm start
require('better-log/install');
var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var knex = require('./backend/utils/knex');
var session = require('express-session');//dev
var hash = require('./backend/utils/password').hash;
app = express();

var loggedinFilter = require('./filters/loggedin');
var fysioFilter = require('./filters/fysio');
var patientFilter = require('./filters/patient');
var adminFilter = require('./filters/admin');

var helmet = require('helmet');
app.use(helmet());
app.ROLE_DOCTOR = 1;
app.ROLE_ADMIN = 2;
app.ROLE_PATIENT = 3;
var routes = require('./routes/index');
var login = require('./routes/login');

var userRoutes = require('./routes/user/main');
var adminRoutes = require('./routes/admin/main');
var fysioRoutes = require('./routes/fysio/main');
var patientRoutes = require('./routes/patient/main');
var apiRoutes = require('./routes/api/main');

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');
//Change this to production when launching website
app.set('env', 'development');

// uncomment after placing your favicon in /public
app.use(favicon(path.join(__dirname,'public','images','favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use(session({
  resave: false, // don't save session if unmodified
  saveUninitialized: false, // don't create session until something stored
  secret: 'sustainSecretKey603'
}));

app.all('/',loggedinFilter);

app.use('/', routes);
app.use('/user', userRoutes);
app.use('/login', login);
app.use('/api', apiRoutes);
// app.use('/patients', fysioFilter,patientsFysio);
app.use('/fysio',fysioFilter,fysioRoutes);
app.use('/admin',adminFilter,adminRoutes);
app.use('/patient',patientFilter,patientRoutes);
// app.use('/progress', progress);
// app.use('/goals', fysioFilter, goals);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handlers

// development error handler
// will print stacktrace
if (app.get('env') === 'development') {
  app.use(function(err, req, res, next) {
    res.status(err.status || 500);
    res.render('error', {
      message: err.message,
      error: err
    });
  });
}

// production error handler
// no stacktraces leaked to user
app.use(function(err, req, res, next) {
  res.status(err.status || 500);
  res.render('error', {
    message: err.message,
    error: {}
  });
});

String.prototype.replaceAll = function(search, replacement) {
    return this.split(search).join(replacement);
};
String.prototype.occurences = function(value) {
  var regExp = new RegExp(value, "gi");
  return (this.match(regExp) || []).length;
}


module.exports = app;
