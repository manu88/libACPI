DefinitionBlock ("", "DSDT", 2, "BXPC", "BXDSDT", 1)
{
    Device(EC0) 
    {
        Name(REGC,Ones)
        Method(_REG,2) 
        {
            If(Lequal(Arg0, 3)) 
            {
                Store(Arg1, REGC)
            }
        } 
    }

    Method(ECAV,0) 
    {
        Return(One)
    }
        

}

