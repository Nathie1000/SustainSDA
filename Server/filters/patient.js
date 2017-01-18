module.exports = function(req,res,next){
  if(!req.session.user || req.session.user.role !== app.ROLE_PATIENT){
    if(req.session.patient && req.session.patient.get('user_id') !== req.session.user.id){ //to make sure the correct user is requesting correct data. Only downside to this is that the call for a current patient has to be made. Upside is this should happen first thing after logging in. 
      console.log('redirecting unlogged in user');
      req.session.user = null;
      res.redirect('/login');
    } else {
      next();
    }
  } else {
    console.log('Doctor is logged in');
    next();
  }
};
