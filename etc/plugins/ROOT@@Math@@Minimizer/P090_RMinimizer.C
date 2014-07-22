void P090_RMinimizer()
{
   gPluginMgr->AddHandler("ROOT::Math::Minimizer", "RMinimizer", "ROOT::Math::RMinimizer",
      "RMinimizer", "RMinimizer(const char *)");
}
