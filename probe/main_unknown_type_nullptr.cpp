class UnkType;

class CreatedByUnkType
{
public:
    CreatedByUnkType(UnkType*) { }
};

class S
{
    class InternalType
    {

    };
public:
    class CreatedByInternalType
    {
    public:
        CreatedByInternalType(InternalType) { }
    };
};

int main()
{
    CreatedByUnkType a(nullptr);
    S s;
    S::CreatedByInternalType s(S::InternalType);
}