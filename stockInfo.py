import requests
import sys
import pandas as pd

def get_shanghai_index(stockindex):
    
    url = 'https://qt.gtimg.cn/q=sh'+stockindex
    response = requests.get(url)
    #data = response.text.split('~')
    return response.text

if __name__ == '__main__':
    if len(sys.argv) > 1:
        stockindex = sys.argv[1]
        #stockindex='   '
        shanghai_index = get_shanghai_index(stockindex)
        print(shanghai_index)
    else:
        print("请输入上证指数代码")
        