# Candy Launcher (v1 Prototype)

Hi, this is an unfinished version 1 of the candy launcher, currently only tested on my laptop (HP Victus). 

## Why I Made It
I wanted to build a fun, automated way to launch candy using computer vision, combining hardware (an ESP32 and mechanical parts) with a custom software setup.

## How to Use It
1. Your laptop must have the physical OMEN Gaming Hub key on the keyboard.
2. Install the Python script and set it to run on startup in Windows.
3. Upload the ESP code to the AI-Thinker ESP32 board.
4. Press the OMEN key to enter a livestream mode where the python script shows a box around your mouth.

> **Note:** As I said, this is still an early prototype. Movement, inverse kinematics, and launching actual candy are not functional yet!
> repo and readme will be updated soon once i test more parts and find some time to write some code.

## CAD
* **CAD Model:** You can check out the design here: [Onshape CAD Document](https://cad.onshape.com/documents/5612db996652adceb7ce30aa/w/0ed39498cd0338a43115c5e1/e/3f8492fab05c535707b619d7)

## (BOM)

| Item | Qty | Description | Source / Link |
| :--- | :--- | :--- | :--- |
| Microcontroller | 1 | AI-Thinker ESP32-CAM (or standard ESP32 board) | [Amazon NL](https://www.amazon.nl/dp/B0D46FHMKQ?ref=ppx_yo2ov_dt_b_fed_asin_title) |
| Host Computer | 1 | HP Victus Laptop (with OMEN Gaming Hub key) | Owner's Hardware |
| USB Programming Cable | 1 | USB-to-MicroUSB/Type-C cable (depending on ESP32 board) | Local store |
| Pcb board | 1, if youre sure you wont mess the milling up;) | single sided | [Amazon NL](https://www.amazon.nl/gp/product/B015PCKPXE/ref=ewc_pr_img_1?smid=AQ1IBDB6G2RRD&psc=1) |
| Connecting Wires | as much as you need, can also use some wires you pull out of broken household machines | some wires | [Amazon NL](https://www.amazon.nl/vaste-gecoat-kleuren-vertind-Fermerry/dp/B09BFFGTLH/ref=sr_1_2_sspa?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=Q8SP6K9C6OOA&dib=eyJ2IjoiMSJ9.pxSsERGflq34Ldfmh0bEjWCfBFKVi2P2MkJH8n2HdjE_Z5dyhoOEvyTAs5koPEEWdsytsEFrM2svgwN4mDXVxYk6irLvgPBiF9l4sM40w70olGPXZfi5wTNxUr2Fi-xZWrQCPlXYnfglaUyYkg3AYYE1DUeBgRe95JcO6Z_Q3TDwo1Ge64b3LyJ7vFWJ34EbcPetVQ26ujkJYBIqSqCywhG0Xb_Bm56Rdi-gJTnZYQ001pfkNm8lbh4kIhD-ICU8jLYV1-Me9TufQufHN2VwoF9BJW59s9hHtkUuw-VDQ7c.6fIoBm1gA-HAufbezBpuelaU2SSmqUJJjVL_m4dBe2I&dib_tag=se&keywords=wires&qid=1786043537&sprefix=wire%2Caps%2C231&sr=8-2-spons&aref=Zewkorpsna&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&th=1) |
| Pla and Tpu | a roll each | for the prints | any online store |
| drone motor | 2 | for the firing mechanism | [AliExpress](https://www.aliexpress.com/p/tesla-landing/index.html?scenario=c_ppc_item_bridge&productId=1005004680039179&_immersiveMode=true&withMainCard=true&src=google-language&aff_platform=true&isdl=y&src=google&albch=shopping&acnt=272-267-0231&isdl=y&slnk=&plac=&mtctp=&albbt=Google_7_shopping&aff_platform=google&aff_short_key=UneMJZVf&gclsrc=aw.ds&albagn=888888&ds_e_adid=&ds_e_matchtype=&ds_e_device=c&ds_e_network=x&ds_e_product_group_id=&ds_e_product_id=en1005004680039179&ds_e_product_merchant_id=633020131&ds_e_product_country=ZZ&ds_e_product_language=en&ds_e_product_channel=online&ds_e_product_store_id=&ds_url_v=2&albcp=23068184797&albag=&isSmbAutoCall=false&needSmbHouyi=false&gad_source=1&gad_campaignid=23058135279&gbraid=0AAAAAoukdWMLSE4_W6v24MqJ2Zod6XktI&gclid=CjwKCAjw4dDTBhAqEiwAkHYmSt8Xy3n7sKOUDD4jQWWoa-zlBC4TF7w6Wapi3mFO9sHIsFYWy1FlkxoC27AQAvD_BwE) |
| power supply | 1 | 5v 6A | [Amazon NL](https://www.amazon.nl/dp/B07P8HKFLL?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1) |
| servo | 3 | 15kg | [Amazon NL](https://www.amazon.nl/dp/B0DHD4J3B4?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1) |
| bolts | quite a few | m3 | [Amazon NL](https://www.amazon.nl/dp/B0DQ17WWGQ?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1) |
| heated inserts | quite a few | m3 | [Amazon NL](https://www.amazon.nl/dp/B0CH32W3W5?ref=ppx_yo2ov_dt_b_fed_asin_title) |
| skittles | depents how much you like skittles | you got to have something to fire | Local store |
