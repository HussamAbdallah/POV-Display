R=imread('image_64_centered.bmp');

k=31.5;      
z=1;
arr = zeros(1,120,'uint8');
theta= zeros(16,16);
rho= zeros(16,16);


for angle= 3:3:360
    for leds = 0:1:31
        
        r_prime = leds + 0.5;
		x= k + (cosd(angle)*r_prime);
		y= - ( (sind(angle)*r_prime) -k);
        
        i= round(x)+1;    
        j= round(y)+1;
       % im_EX(i,j) = R(i,j);
        
        if R(j,i) == 0 
            theta(j,i)= angle*0.01745329252;
            rho(j,i) = r_prime;
         
            arr(z) = arr(z) + (2^leds);
        end
      
        
    end 
    z= z+1;
  
 end


[r,c] = size(arr);
%% To Give array suitable for the C language
arr1=sprintf([ '[' [repmat('%d,', 1, c-1) repmat('%d', 1, 1)] ']'], arr')
polar(theta,rho,'o')

