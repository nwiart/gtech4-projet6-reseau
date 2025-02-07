Client& Client::getInstance()
{
	static Client inst;
	return inst;
}
