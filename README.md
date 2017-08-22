# stegonography
stegonography is the technic that used to hide the message into image file.without knowing to the other person accept sender and receiver.
Text can be send in the form of image that can be received by the receiver using password.
The project mainly had two operations.
    1) Encryption
    2) Decryption
Algorithm : Encoding
Step1 : start
Step2 : Read the
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
