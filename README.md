# stegonography

stegonography is the technic that used to hide the message into image file.without knowing to the other person accept sender and receiver.
Text can be send in the form of image that can be received by the receiver using password.

The project mainly had two operations.
    1) Encryption
    2) Decryption
    
Algorithm : Encoding

Step1 : start

Step2 : Read the files

       1.Text file    
       2.Image file
       3.Destination file
       
Step3 : Create password

Step4 : open the all 3 files

Step5 : Get size of text file and Image file and compare.means check capacity of image file .size of image file should be greater 
        than size of text file then only encoding possible
        
Step6 : copy the header from image file to destination file.

Step7 : Encode the magicstring

Step8 : Encode the text file password size and password

Step9 : Encode the text file extention

Step10 :Encode the text file size

Step11 : Encode the text file data.

Step12 : copy the remaining data from text file and encode it byusing the bytes read from the image file and write into the 
         destination file.
         
Step13 : Stop         

Algorithm : Decoding

Step1 : Start

Step2 : Read files

       1. Text file
       2. Encoded image file
       
Step3 : Read the password

Step4 : Open the files

Step5 : Decode the magic string

Step6 : Decode the password size and password

Step7 : Compare the decoded password and entered password if it is not matches then go to stop

Step8 : Decode secret file extention.

Step9 : Decode the secret file size

Step10 : Decode the secret file data

Step11 :Store the data into text file

Step12 :Remaining destination file will be image file.

Step13 : Stop.


Advantages :

1. More security.

2. There is no difference between orginal image and encoded image.

Disadvantage:

1. Size of text file always less than image file.

2.encoded data will distroyed if image is compressed.
