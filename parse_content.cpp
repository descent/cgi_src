#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

//#define CGI_SAVE_INPUT

string get_file_content(const string &str)
{
  string ret_str;

  const char pattern[] = {0x0d, 0x0a, 0x0d, 0x0a, 0}; // \r\n\r\n
  const char end_pattern[] = {0x0d, 0x0a, 0}; // \r\n
  string::size_type pos = str.find(pattern);
  if (pos != string::npos)
  {
    pos += strlen(pattern);
    string::size_type end = str.find(end_pattern, pos);
    if (end != string::npos)
    {
      ret_str = str.substr(pos, end-pos);
    }
  }
  return ret_str;
}

char *get_file_content(char *content, int &len)
{
  const char pattern[] = {0x0d, 0x0a, 0x0d, 0x0a, 0}; // \r\n\r\n
  const char end_pattern[] = {0x0d, 0x0a, 0}; // \r\n
  char *fn = strstr(content, pattern);

  if (fn)
  {
    fn = fn + strlen(pattern); // text/plain\r\n\r\n
#ifdef CGI_DEBUG
    cout << "sizeof(pattern): " << sizeof(pattern) << endl;
    cout << "strlen(pattern): " << strlen(pattern) << endl;
    cout << "xxx fn: " << fn << endl;
#endif
    char *end = strstr(fn, end_pattern);
    len = end - fn;
    if (end)
      *end = 0;
    else
      return 0;
  }
  return fn;
}

string get_fn(const string &str)
{
  string ret_str;

  const char *pattern = "filename=";
  string::size_type pos = str.find(pattern);
  if (pos != string::npos)
  {
    pos += strlen(pattern) + 1;
    string::size_type end = str.find('\"', pos);
    if (end != string::npos)
    {
      ret_str = str.substr(pos, end-pos);
    }
  }
  return ret_str;
}

#if 0
char *get_fn(char *content)
{
  char *fn = strstr(content, pattern);
  if (fn)
  {
    fn = fn + strlen(pattern) + 1; // filename="t1.txt"
#ifdef CGI_DEBUG
    cout << "sizeof(pattern): " << sizeof(pattern) << endl;
    cout << "strlen(pattern): " << strlen(pattern) << endl;
    cout << "xxx fn: " << fn << endl;
#endif
    char *end = strchr(fn, '\"');
    if (end)
      *end = 0;
    else
      return 0;
  }
  return fn;
}
#endif

//cgi variable: http://www.lyinfo.net.cn/webclass/cgi/cgiclass5-2.htm
int main(int argc, char *argv[])
{
  string mark = "----------1929521779952211446209360987";

  FILE *fs = fopen("./f", "r");
  if (fs == NULL) return -1;

  fseek(fs, 0L, SEEK_END);
  int read_len = ftell(fs);
  fseek(fs, 0L, SEEK_SET);
  cout << "read_len: " << read_len << endl;

  char *buf = new char[read_len];
  int count = fread(buf, 1, read_len, fs);


  string::size_type pos = 0;

  string str_buf(buf, read_len);

  delete [] buf;

  char *token=0;
  char *ptr = buf;
#if 0
  token = strstr(buf_str, mark.c_str() );
  if (token != NULL)
    cout << "token: " << token << endl;
#endif


  vector<string::size_type> idx_vec;

  while(1)
  {
#if 0
    token = strstr(ptr, mark.c_str() );
    if (token == NULL)
      break;
#endif
    pos = str_buf.find(mark, pos);
    if (pos == string::npos)
      break;


#if 0
    //cout << "\ttoken: " << token + mark.size() << endl;
    idx_vec.push_back(token + mark.size());
    ptr = token + mark.size();
    *token = 0;
#endif

    pos += mark.size();
    idx_vec.push_back(pos);
  }
  cout << "zz: " << idx_vec.size() << endl;

  vector<string> blocks;
  for (int i = 0 ; i < idx_vec.size()-1 ; ++i)
  {
    //cout << "sub len: " << idx_vec[i] << endl;
    string::size_type len = idx_vec[i+1] - idx_vec[i];
    blocks.push_back(str_buf.substr(idx_vec[i], len));
  }

  for (int i = 0 ; i < blocks.size() ; ++i)
  {
    string fn = get_fn(blocks[i]);
    cout << "fn: " << fn << endl;
    string fc = get_file_content(blocks[i]);
    ofstream ofile("/tmp/2");
    ofile << fc;
  }

#if 0

  for (int i = 0 ; i < idx_vec.size()-1 ; ++i)
  {
    cout << "  substr: " << idx_vec[i] << endl;
    int count;
    char *fc = get_file_content(idx_vec[i], count);
    cout << "    file len: " << count << endl;
    cout << "    fc: " << fc << endl;

    char *fn = get_fn(idx_vec[i]);
    cout << "    fn: " << fn << endl;

    FILE *fs;
    std::string dir_path("/tmp/");
    std::string fpath = dir_path + fn;
    cout << "    fpath: " << fpath << endl;

    fs = fopen(fpath.c_str() , "w");
    fwrite(fc, 1, count, fs);
    fclose(fs);
  }
#endif

#if 0
  while(1)
  {
    pos = buf_str.find(mark, pos);
    pos += mark.size();
    cout << "pos: " << pos << endl;
    if (string::npos == pos)
      break;
    idx_vec.push_back(pos);
  }
  for (int i=0 ; i < idx_vec.size() ; ++i)
  {
    cout << "#" << i << ": " << idx_vec[i];
  }
#endif
#if 0
  env = getenv("CONTENT_FILE");
  if (env)
    printf("env: %s<br>\n", env);

  env = getenv("REQUEST_METHOD");
  if (env)
    printf("env: %s<br>\n", env);
#endif
  return 0;
}
