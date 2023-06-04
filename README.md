# Yet-Another-Trie-To-Hash-Image ![C++](https://img.shields.io/badge/-C++-505050?logo=c%2B%2B&style=flat)
### Image Hashing &amp; Retrieval algorithm proposed for "Introduction to Computer Vision - SNU" term project, on June 2023
<hr/>
<p align="center">
  <img src="docs/images/fig1.png", style="width:80%">
</p>
Most existing algorithms follow a two-step approach, where they first extract features from an image and then compare the feature vector of the input image with those stored in the database. To search the query image more efficiently, hashing techniques have traditionally been chosen as solutions. These techniques typically involve feature extraction, followed by quantization of the values, resulting in a quantized string that serves as the hash.
<br/><br/><br/>
<p align="center">
  <img src="docs/images/fig2.png", style="width:80%;margin-left:auto;margin-right:auto;display:block;">
</p>
Hashing techniques, however, require an "exact match" between hash values, which can lead to weaknesses when dealing with certain variants, such as affine transforms and hue shifts.
<br/><br/><br/>
<p align="center">
  <img src="docs/images/fig3.png", style="width:60%;margin-left:auto;margin-right:auto;display:block;">
</p>
In this paper, we introduce quantization margin variable, ξ, allowing values in the vicinity of 0.5 to match both 0 and 1. 
<br/><br/><br/>
<p align="center">
  <img src="docs/images/fig4.png", style="width:60%;margin-left:auto;margin-right:auto;display:block;">
</p>
The margin makes the quantization robust to variations, ensuring that the altered hash is compatible with the original one.
<br/><br/><br/>
<p align="center">
  <img src="docs/images/fig5.png", style="width:60%;margin-left:auto;margin-right:auto;display:block;">
</p>
Indeed, the quantized hash value can be stored and searched efficiently by "Trie" data structure. It has O(log N) time complexity in average case, and guarantees O(N) time complexity in worst case.

# Paper

**The paper is currently a WIP.**

# License

**The MIT License (MIT)**

**Copyright (c) 2023 changdae20.**
