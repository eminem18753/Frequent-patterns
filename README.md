<span style="font-size:32px;"><b>Frequent patterns</b></span> <br />
Part 1.目標: <br />
1.找出顧客交易資訊中大於min support的顧客購買商品組合 <br />
->min support的定義可參考<a href="https://medium.com/marketingdatascience/%E4%BD%A0%E6%80%8E%E9%BA%BC%E8%99%95%E7%90%86%E9%A1%A7%E5%AE%A2%E4%BA%A4%E6%98%93%E8%B3%87%E8%A8%8A-apriori%E6%BC%94%E7%AE%97%E6%B3%95-1523b1f8443b">連結</a> <br />
Part 2.解釋sample.in: <br />
1.每一列代表1組商品的購買(該購物者該次可能購買多個商品，所有購買的商品id以逗號區隔) <br />
Part 3.執行方式: <br />
1.於windows系統打開cmd <br />
2.執行"mingw32-make -f makefile" <br />
3.執行".\main.exe 0.1 sample.in sample_0.1.out"，會輸出檔案sample_0.1.out <br />
->其中0.1為min support <br />
