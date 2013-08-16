#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <vector>
#include <string>

using namespace std;

//cgi variable: http://www.lyinfo.net.cn/webclass/cgi/cgiclass5-2.htm
int main(int argc, char *argv[])
{
  cout << "Content-type: text/plain\n\n";
  cout << "cgi test<br>\n";

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

  env = getenv("CONTENT_LENGTH");
  int read_len;
  char *buf;
  if (env)
  {
    read_len = atoi(env);
    buf = new char[read_len];
    int count = fread(buf, 1, read_len, stdin);
    cout << "buf:\n";
    fwrite(buf, 1, count, stdout);
    //printf("CONTENT_LENGTH str: %s\n", env);
    //printf("CONTENT_LENGTH num: %d\n", read_len);
  }

  //printf("buf: %s\n", buf);

  //std::string buf_str(buf, read_len);

#if 0
  mark = "-----------------------------50320045514440238611475056647";
  //char buf_str[3000]= "-----------------------------50320045514440238611475056647 Content-Type: text/plain 123 -----------------------------50320045514440238611475056647 Content-Type: text/plain zyx -----------------------------50320045514440238611475056647 Content-Type: text/plain 1q2w -----------------------------50320045514440238611475056647--";
  char buf_str[2000]= "-----------------------------50320045514440238611475056647 Content-Type: text/plain 123 -----------------------------50320045514440238611475056647 Content-Type: text/plain zyx -----------------------------50320045514440238611475056647 Content-Type: text/plain 1q2w -----------------------------50320045514440238611475056647--";
#endif

  //cout << buf_str << endl;


  vector<char *> idx_vec;
  string::size_type pos = 0;

  char *token=0;
  char *ptr = buf;
#if 0
  token = strstr(buf_str, mark.c_str() );
  if (token != NULL)
    cout << "token: " << token << endl;
#endif

  while(1)
  {
    token = strstr(ptr, mark.c_str() );
    if (token == NULL)
      break;

    //cout << "\ttoken: " << token + mark.size() << endl;
    idx_vec.push_back(token + mark.size());
    ptr = token + mark.size();
    *token = 0;
  }

  for (int i = 0 ; i < idx_vec.size()-1 ; ++i)
  {
    cout << "  substr: " << idx_vec[i] << endl;
  }

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
  delete [] buf;
  return 0;
}
