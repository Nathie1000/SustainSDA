module.exports = function(req,res,next){
  if(!req.session.user || req.session.user.role !== app.ROLE_DOCTOR){
    console.log('redirecting unlogged in user');
    req.session.user = null;
    res.redirect('/login');
  } else {
    console.log('Doctor is logged in');
    next();
  }
};
