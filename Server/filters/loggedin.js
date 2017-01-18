module.exports = function(req,res,next){
  if(!req.session.user){
    console.log('redirecting unlogged in user');
    req.session.user = null;
    res.redirect('/login');
  } else {
    next();
  }
};
