
#define SBUFFER_SIZE 256

class sbuffer : public Stream
{
    public:
    int begin();
    int available();
    int read();
    int peek();
    void flush();
    size_t write(uint8_t c);
    size_t write(const uint8_t *buffer, size_t size);

    //Custom functions:
    bool isTimeout();
    int getwi();
    int getri();
    
    protected:
    char buffer[SBUFFER_SIZE];
    int _ava = 0; 
    int _ri = 0;
    int _wi = 0;
};

int sbuffer::begin()
{
    _ava = 0; 
    _ri = 0;
    _wi =0;
}

int sbuffer::available()
{
  return(_ava);
}
int sbuffer::read()
{
  int ret;
  if(_ava > 0)
  {    
    ret = buffer[_ri];
    _ri = (_ri + 1)%SBUFFER_SIZE;
    _ava--;
    return(ret);
  }
  else return(-1);
}
int sbuffer::peek()
{
  return(buffer[_ri]);
}
void sbuffer::flush()
{
  //return(_ava > 0); //use external loop until condition
}
size_t sbuffer::write(uint8_t c)
{
  if(_ava < SBUFFER_SIZE)
  {
    buffer[_wi] = c;
    _wi = (_wi + 1)%SBUFFER_SIZE;
    _ava++;
    return 0;
  }
  return 1; 
}
size_t sbuffer::write(const uint8_t *buffer, size_t size)
{
    size_t n = 0;
    while(size--) {
        n += write(*buffer++);
    }
    return n;
}
bool sbuffer::isTimeout()
{
  return((_timeout > 0) && (millis() >= _timeout));
}
int sbuffer::getwi() 
{
  return(_wi);
}

int sbuffer::getri()
{
  return(_ri);
}
