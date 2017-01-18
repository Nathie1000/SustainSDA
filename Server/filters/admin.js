module.exports = function(req,res,next){
  if(!req.session.user || req.session.user.role !== app.ROLE_ADMIN){
    console.log('redirecting unlogged in user');
    req.session.user = null;
    res.redirect('/login');
  } else {
    console.log('Admin is logged in');
    next();
  }
};
