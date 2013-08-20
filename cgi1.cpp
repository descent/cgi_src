#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define CGI_SAVE_INPUT

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

char *get_fn(char *content)
{
  const char *pattern = "filename=";
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

//cgi variable: http://www.lyinfo.net.cn/webclass/cgi/cgiclass5-2.htm
int main(int argc, char *argv[])
{
  cout << "Content-type: text/plain\n\n";
  cout << "cgi test<br>\n";
    //return -1;

  char *env = getenv("CONTENT_TYPE");
  string bound, mark;
  if (env)
  {
    printf("CONTENT_TYPE: %s\n", env);
    bound = strstr(env, "boundary");
    string::size_type index = bound.find("=");
    mark = bound.substr(index+1, bound.size() ); // add 1 for "="
    cout << "mark: " << mark << endl;
  }
  else
  {
    return -1;
  }

  env = getenv("CONTENT_LENGTH");
  int read_len;
  char *buf;
  if (env)
  {
    read_len = atoi(env);
    cout << "read len: " << read_len << endl;
    buf = new char[read_len];
    int count = fread(buf, 1, read_len, stdin);


#ifdef CGI_DEBUG
    cout << "buf:\n";
    fwrite(buf, 1, count, stdout);
#endif
#ifdef CGI_SAVE_INPUT
    FILE *fs;
    const char *sfn = "/tmp/f";
    fs = fopen(sfn, "w");
    fwrite(buf, 1, count, fs);
    fclose(fs);
    cout << "save all input to " << sfn;
    return 0;
#endif

    //printf("CONTENT_LENGTH str: %s\n", env);
    //printf("CONTENT_LENGTH num: %d\n", read_len);
  }

  string str_buf(buf, read_len);
  delete [] buf;

  vector<string::size_type> idx_vec;

  string::size_type pos = 0;
  while(1)
  {
    pos = str_buf.find(mark, pos);
    if (pos == string::npos)
      break;

    pos += mark.size();
    idx_vec.push_back(pos);
  }
  //cout << "zz: " << idx_vec.size() << endl;

  vector<string> blocks;
  for (int i = 0 ; i < idx_vec.size()-1 ; ++i)
  {
    cout << "sub len: " << idx_vec[i] << endl;
    string::size_type len = idx_vec[i+1] - idx_vec[i];
    blocks.push_back(str_buf.substr(idx_vec[i], len));
  }

  for (int i = 0 ; i < blocks.size() ; ++i)
  {
    string path = "/tmp/";
    string fn = get_fn(blocks[i]);
    cout << "fn: " << fn << endl;
    string fc = get_file_content(blocks[i]);
    ofstream ofile((path+fn).c_str() );
    ofile << fc;
    cout << "fn len: " <<  fc.length() << endl;
  }


  return 0;
}
