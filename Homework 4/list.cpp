
void listAll(string path, const Domain* d) // two-parameter overload
{
    if((d->subdomains().size()) == 0)
    {
        cout << path << endl;
        return ;
    }
    for (vector<Domain*>::const_iterator p = d->subdomains().begin(); p != d->subdomains().end(); p++)
        path != "" ? listAll((*p)->label() + "." + path, *p) : listAll((*p)->label() + path, *p);
}
