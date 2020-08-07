#include <bits/stdc++.h>
#include <fstream>
using namespace std;
#define ll long long
#define M 1000000007
#define Block_Size 8
ll no_of_hits=0;
ll no_of_misses=0;
ll cache[256][Block_Size];
ll tag_register[256];
ll valid[256][Block_Size];
ll dirty_bit[256];
string integer_to_binary(ll n)
{
    string s = "";
    int k=0;
    while(n!=0)
    {
        k++;
        s.push_back(n%2+'0');
        n/=2;
    }
    while(s.size()<32)
    {
        s.push_back('0');
    }
    reverse(s.begin(),s.end());
    return s;
}
ll binary_to_lleger(string s)
{
    //cout<<s.size()<<endl;
    ll ans=0;
    ll x=1;
    for(ll i=s.size()-1;i>=0;i--)
    {
        if(s[i]-'0')
        {
            ans+=x;
        }
        x=x*2;
    }
    return ans;
}
ll extract_from_memory(ll address)
{
    ifstream infile;
    infile.open("memory.txt");
    ll c=0;
    string temp;
    while(c<address+1)
    {
        infile>>temp;
        c++;
    }
    return binary_to_lleger(temp);
}
ll extract_offset(ll address)
{
    return address%Block_Size;
}
ll extract_index(ll address)
{
    address=address/Block_Size;
    return address%256;
}
ll extract_tag(ll address)
{
    address=address/pow(2,8+(int)log2(Block_Size));
    return address;
}
bool hit(ll address)
{
    ll temp=address;
    ll offset=extract_offset(address);
    ll index=extract_index(address);
    ll tag=extract_tag(address);
    if(valid[index][offset]==0)
    {
        return false;
    }
    if(tag_register[index]==tag)
    {
        return true;
    }
    return false;
}
ll load(ll address)
{
    ll index=extract_index(address);
    ll offset=extract_offset(address);
    ll tag=extract_tag(address);
    if(hit(address))
    {
        no_of_hits++;
        return cache[index][offset];
    }
    else
    {
        tag_register[index]=tag;
        valid[index][offset]=1;
        ll ans=extract_from_memory(address);
        if(dirty_bit[index]==0)
        {
            tag_register[index]=tag;
            valid[index][offset]=1;
            ll x=address-address%Block_Size;
            for(ll i=0;i<Block_Size;i++)
            {
                cache[index][i]=extract_from_memory(x+i);
            }
        }
        else
        {
            fstream fio;
            fio.open("memory.txt");
            ll k=0;
            ll x=address-address%Block_Size;
            while(k<x)
            {
                string line;
                fio>>line;
                k++;
            }
            for(ll i=0;i<Block_Size;i++)
            {
                fio<<integer_to_binary(cache[index][i])<<endl; //overwriting doesn't work fine in cpp
            }
            fio.close();
            tag_register[index]=extract_tag(address);
            for(ll i=0;i<Block_Size;i++)
            {
                cache[index][i]=extract_from_memory(x+i);
            }
        }
        valid[index][offset]=1;
        no_of_misses++;
        return ans;
    }
}
void store(ll address)//as we didn't know what value to store, we stored 0
{
    if(hit(address))
    {
        no_of_hits++;
        ll index = extract_index(address);
        dirty_bit[index]=1;
        ll offset = extract_offset(address);
        cache[index][offset]=0;
    }
    else
    {
        no_of_misses++;
        ll index = extract_index(address);
        ll offset = extract_offset(address);
        if(dirty_bit[index]==1)
        {
            fstream fio;
            fio.open("memory.txt");
            ll k=0;
            ll x=address-address%Block_Size;
            while(k<x)
            {
                string line;
                fio>>line;
                k++;
            }
            for(ll i=0;i<Block_Size;i++)
            {
                fio<<integer_to_binary(cache[index][i])<<endl;//overwriting doesn't work fine in cpp
            }
            fio.close();
            tag_register[index]=extract_tag(address);

            for(ll i=0;i<Block_Size;i++)
            {
                cache[index][i]=extract_from_memory(x+i);
            }
            cache[index][offset]=0;
            valid[index][offset]=1;
        }
        else
        {

            ll x=address-address%Block_Size;
            for(ll i=0;i<Block_Size;i++)
            {
                cache[index][i]=extract_from_memory(x+i);
            }
            tag_register[index]=extract_tag(address);
            cache[index][offset]=0;
            dirty_bit[index]=1;
            valid[index][offset]=1;
        }
        
    }
}
ll hexa_to_integer(string s)
{
    //cout<<s.size()<<endl;
    ll ans=0;
    ll x=1;
    for(ll i=s.size()-1;i>=0;i--)
    {
        if(s[i]>=48&&s[i]<58)
        {
            ans+=x*(s[i]-48);
        }
        else
        {
            ans+=x*(s[i]-'a'+10);
        }
        
        x=x*16;
    }
    return ans;
}
int main()
{
    cout<<log2(Block_Size)<<endl;
/*    store(4);
    cout<<cache[0][extract_offset(4)]<<endl;
    cout<<no_of_hits<<endl;
    cout<<load(4)<<endl;
    */
    ll c=0;
    ll c2=0;
    //cout<<extract_from_memory(3)<<endl;
    //cout<<load(3)<<endl;
    fstream input;
    input.open("swim.trace");
    while(input)
    {
        c2++;
        string type;
        input>>type;
        string address;
        input>>address;
        string temp;
        input>>temp;
        //cout<<address<<endl;
        if(address.length()>2)
        {
            address = address.substr(2,address.length());
            //cout<<address<<endl;
            ll addr = hexa_to_integer(address);
            if(addr<99999)
            {
                c++;
                if(type=="s")
                    store(addr);   
                else
                    cout<<load(addr)<<endl;
            }
        }
    }
    cout<<c*1.0/c2<<endl;
    cout<<no_of_hits*1.0/(no_of_hits+no_of_misses)<<endl;
}