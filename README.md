
# Candy Launcher V1
<img width="881" height="250" alt="Screenshot 2026-07-14 200553" src="https://github.com/user-attachments/assets/2128f9b7-e7f5-411d-afde-c022e2bd4ecc" />

Hi, this is a finished v1 of the candy launcher, currently only tested on my laptop (HP Victus). 


<img width="1496" height="1987" alt="IMG_20260830_213125606" src="https://github.com/user-attachments/assets/032c435d-f456-4237-a53d-4acf272ce370" />


link to video with more details:
[https://www.youtube.com/watch?v=WshQ-3ue9KE](https://www.youtube.com/watch?v=Oau0cctOxF0)

## Why I Made It
I wanted to build a fun, automated way to launch candy using computer vision, combining hardware (an ESP32 and mechanical parts) with a custom software setup.

## Future updates will bring:
1. improved face tracking, one that also work in dim light
2. improved IK (there seems to be a bug that stops it from following to the full range)
3. improved shot consitency
4. other candy options

## Requirements

### Python

The python script was written with Python 3 and currently only tested on Windows.

Install the required python libraries:

```bash
pip install psutil requests opencv-python numpy
```

### OpenCV

OpenCV already includes:

- `haarcascade_frontalface_default.xml`

You still need to download:

- `haarcascade_mcs_mouth.xml`

and place it in the same folder as the python script.

### ESP32

This project uses an AI-Thinker ESP32-CAM.

Install the ESP32 by Espressif Systems board package through the Arduino IDE Boards Manager.

The following libraries are already included with the board package, so you don't have to install anything extra:

- WiFi
- esp_camera
- esp_http_server

The sketch also expects a `board_config.h` file for the camera pin definitions.

## How to Use It

1. Your laptop must have the physical OMEN Gaming Hub key on the keyboard.
2. Install Python and the required libraries shown above.
3. Download `haarcascade_mcs_mouth.xml` and put it next to the python script.
4. Install the ESP32 board package in the Arduino IDE.
5. Upload the ESP code to the AI-Thinker ESP32 board.
6. Change the Wi-Fi name and password in the ESP sketch.
7. Set the python script to run on Windows startup.
8. Press the OMEN key to enter livestream mode where the python script shows a box around your mouth.

> **Note:** Other esp code is purely for debugging hardware, if something is not working, test the respective section with that code

## CAD
* **CAD Model:** You can check out the design here: [Onshape CAD Document](https://cad.onshape.com/documents/5612db996652adceb7ce30aa/w/0ed39498cd0338a43115c5e1/e/3f8492fab05c535707b619d7)

## Assembly and explanation

> Assembly steps are not set in stone and there are multiple ways to Rome, this is just how i did it. :)
1. mill the pcb out of 1.6mm copper clad, 3d print all the files in the stl folder(make sure the tube and flywheels are printed in tpu)(also make sure that the lid and container are paused at the correct layer and insert the magnets)
2. insert heated inserts in all the holes in the base and solder all components to the pcb
3. mount the servos horns and servos as shown in the picture and bolt them in place
<img width="660" height="461" alt="image" src="https://github.com/user-attachments/assets/b5a09730-dc68-451b-8244-07dd6c02d9c8" />

>the 3 servos will be used in sync to tilt the platform towards your mouth
4. insert the balljoints in the circular cutout in the pushrods(should be a press fit), and interlock the side with the fingers with the pushrods with the servo horns and put a bolt through.
> should be a low friction fit

5. mount the pcb with washers as spacers to the platform as shown below. also mount the slide(needs heated inserts first), also mount the motor and the flywheel(all pressfits but can use some glue)
<img width="514" height="475" alt="image" src="https://github.com/user-attachments/assets/19a69e88-240f-48f4-86c3-f4c50ccf0d88" />

6. install some heated inserts in the container, and mount the cap, slider, servo and servo gear(servo with hot glue), also mount the small dc motor with hot glue in the recess in the cap. and screw the printed weight on the motor shaft
<img width="607" height="438" alt="image" src="https://github.com/user-attachments/assets/cf15a9b6-bddf-493b-b331-f3adea3a98c7" />

> make sure the servo gear is not installed when first turned on because the servo might jerk to 0 on power on.
> 
> the servo and motor work together to shake the skittles and feed them one at a time
7. connect all the wires according to the diagram
8. snap the tube into the cap and the slide to connect the 2 assemblies and mount on a wall with screws.
<img width="1918" height="1444" alt="IMG_20260814_225249038" src="https://github.com/user-attachments/assets/dd104330-172c-4e08-a207-aa696636ec28" />

9. DONE!!

## Wiring
<img width="3433" height="1861" alt="Candy launcer wiring diagram" src="https://github.com/user-attachments/assets/2799cc04-1d00-422d-9171-e6481ff3958b" />



## BOM

> Prices are in USD converted from EUR, so they might not be 100% accurate, especially with American websites.

| Item | Qty | Price (USD from EUR, might not be accurate with American websites) | Description | Source / Link |
| :--- | :--- | ---: | :--- | :--- |
| Microcontroller | 1 | 11.55 | AI-Thinker ESP32-CAM | [Link](https://www.amazon.nl/dp/B0D46FHMKQ?ref=ppx_yo2ov_dt_b_fed_asin_title) |
| Host Computer | 1 | 1848.36 | HP Victus Laptop (with OMEN Gaming Hub key) | Owner's Hardware |
| USB Programming Cable | 1 | 10.39 | USB-to-Type-C cable | [Link](https://www.amazon.nl/SUNGUY-USB-compatibel-Android-Auto/dp/B0B28ZCV2Y/ref=asc_df_B0B28ZCV2Y?mcid=637835adbbfa3ec5b925935d5940d08b&tag=nlshogostdde-21&linkCode=df0&hvadid=709872675246&hvpos=&hvnetw=g&hvrand=5630318727742800812&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9065081&hvtargid=pla-1725998624389&hvocijid=5630318727742800812-B0B28ZCV2Y-&hvexpln=0&gad_source=1&th=1) |
| Pcb board | 1, if youre sure you wont mess the milling up;) | 19.36 | single sided | [Link](https://www.amazon.nl/gp/product/B015PCKPXE/ref=ewc_pr_img_1?smid=AQ1IBDB6G2RRD&psc=1) |
| Connecting Wires | as much as you need, can also use some wires you pull out of broken household machines | 0-22.53 | some wires | [Link](https://www.amazon.nl/vaste-gecoat-kleuren-vertind-Fermerry/dp/B09BFFGTLH/ref=sr_1_2_sspa?__mk_nl_NL=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=Q8SP6K9C6OOA&dib=eyJ2IjoiMSJ9.pxSsERGflq34Ldfmh0bEjWCfBFKVi2P2MkJH8n2HdjE_Z5dyhoOEvyTAs5koPEEWdsytsEFrM2svgwN4mDXVxYk6irLvgPBiF9l4sM40w70olGPXZfi5wTNxUr2Fi-xZWrQCPlXYnfglaUyYkg3AYYE1DUeBgRe95JcO6Z_Q3TDwo1Ge64b3LyJ7vFWJ34EbcPetVQ26ujkJYBIqSqCywhG0Xb_Bm56Rdi-gJTnZYQ001pfkNm8lbh4kIhD-ICU8jLYV1-Me9TufQufHN2VwoF9BJW59s9hHtkUuw-VDQ7c.6fIoBm1gA-HAufbezBpuelaU2SSmqUJJjVL_m4dBe2I&dib_tag=se&keywords=wires&qid=1786043537&sprefix=wire%2Caps%2C231&sr=8-2-spons&aref=Zewkorpsna&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&th=1) |
| Pla and Tpu  | a roll each | about 44   | for the prints | any online store  |
| power supply | 1 | 18.5 | 5v 6A | [Link](https://www.amazon.nl/dp/B07P8HKFLL?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1) |
| servo | 3 | 25.43 | 15kg | [Link](https://www.amazon.nl/dp/B0DHD4J3B4?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1) |
| bolts  | quite a few | 10.4 | m3 | [Link](https://www.amazon.nl/dp/B0DQ17WWGQ?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1) |
| heated inserts | quite a few | 7.51 | m3 | [Link](https://www.amazon.nl/dp/B0CH32W3W5?ref=ppx_yo2ov_dt_b_fed_asin_title) |
| skittles | depents how much you like skittles | 2.76/pack(150g) | you got to have something to fire | Local store |
| mosfed | 1 | 3.83 | IRL3103 | [Link](https://nl.aliexpress.com/item/1005007481357436.html?spm=a2g0o.order_list.order_list_main.5.4e201802sEbdc8&gatewayAdapt=glo2nld) |
| Capacitor | 1 | 3 | 1000uf, >16v | [Link](https://nl.aliexpress.com/item/1005012180336454.html?spm=a2g0o.productlist.main.50.7de07e2f0ghaGb&algo_pvid=950b2ce2-c73a-4b83-8e77-0756328248c1&algo_exp_id=950b2ce2-c73a-4b83-8e77-0756328248c1-49&pdp_ext_f=%7B%22order%22%3A%2287%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%215.41%212.60%21%21%2141.20%2119.78%21%400b0fe40d17881700344927078e0d77%2112000057690685578%21sea%21NL%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3A22d239ba%3Bm03_new_user%3A-29895&curPageLogUid=vdBU0shoRl3v&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005012180336454%7C_p_origin_prod%3A) |
| magnets, optional | 12 | 5 | 2mm by 4 mm | [Link](https://nl.aliexpress.com/item/1005012419891895.html?spm=a2g0o.productlist.main.10.42c1WbBBWbBBQz&algo_pvid=414dfc03-60bf-48cc-89db-c1cfbde6e9cc&algo_exp_id=414dfc03-60bf-48cc-89db-c1cfbde6e9cc-9&pdp_ext_f=%7B%22order%22%3A%221%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%217.41%215.19%21%21%218.34%215.84%21%400b88a95617861398254598606e0e39%2112000058323417834%21sea%21NL%216320108034%21X%211%210%21n_tag%3A-29919%3Bd%3A22d239ba%3Bm03_new_user%3A-29895&curPageLogUid=4vGJzhw6EWXI&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005012419891895%7C_p_origin_prod%3A) |
| cheap dc motor 5v | 2 | 6 | for shaking and launching | [Link](https://www.amazon.nl/JZK-mini-motor-micro-elektrische-doe-het-zelf-afstandsbediening/dp/B09TRFBSFV/ref=asc_df_B09TRFBSFV?mcid=73ad890f7ca3382db8bfe834d032bc08&tag=nlshogostdde-21&linkCode=df0&hvadid=710040633529&hvpos=&hvnetw=g&hvrand=2095630046737433761&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9065081&hvtargid=pla-1927750089039&psc=1&hvocijid=2095630046737433761-B09TRFBSFV-&hvexpln=0&gad_source=1) |
| 9g servo | 1 | 10 | not a lot of torque is needed | [Link](https://www.amazon.nl/Servomotor-Accessoires-Prestatie-Helikopters-Vliegtuigen/dp/B0GXZNC7WV/ref=sr_1_1_sspa?crid=X0IBSUGGUNJ5&dib=eyJ2IjoiMSJ9.PZzBll0_XNHFFyXfAC9YUxTb7ldssHwt6YpYfmmo_oPWmeXYdzCC_GLGRL3fFYJ3NbHGnZ20lwAHFPIxiV5Knl5IJPZ8N8a3Isp8fu_eNfoA-rv-AzCdF4qOzJCZOAj5BaTMrAfRY-77D3575Ke99qX4Rp3aTBdAa9CHuMzQpwxpibWmwpzDnhrdn3C-IYXi90FOQIcnibooidAWdEwTJ1uBpH6BHFMEzthLemcV9xaYPp2kLi25scamPpv1OJXMkPE5mOtLp7NTwPl3oCz8zOCIXWFF0OQMuHxdvem9_xk.Bpg461fEyAy3N8YEaPzivcYQWv0fXQjFz-X4SffsHNU&dib_tag=se&keywords=9g+servo&qid=1786141027&sprefix=9g+%2Caps%2C103&sr=8-1-spons&aref=dvB2hYBFXB&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1) |
| rc ball joints  | 3 | 9 | for the platform | [Link](https://www.amazon.nl/dp/B097H25M92?ref=ppx_yo2ov_dt_b_fed_asin_title) |
| resistor 10k ohm | 1 | 0.15 | for the mosfed | [Link](https://www.bitsandparts.nl/weerstand-10k-ohm-1-4w-5pct-p101270?gad_source=1&gad_campaignid=17339301268&gbraid=0AAAAADpItpfYr-Ik_2fPAcqy_Rc_vzIFo&gclid=CjwKCAjwzNTUBhAjEiwA7zcvWsYjOvEhi_zSSjWNqMEkyLuZJBGUrrZHBPv-uGUrDBViLvkJUtA0bxoCoB0QAvD_BwE) |
| headers | quite a few |  | to plug everything in | [Link](https://www.amazon.nl/Glarks-Mannelijke-Vrouwelijke-Assortiment-Stapelbare/dp/B09BDX69VX/ref=asc_df_B09BDX69VX?mcid=b79b256c904836bf95e324e2424301f2&tag=nlshogostdde-21&linkCode=df0&hvadid=709885038928&hvpos=&hvnetw=g&hvrand=12931435962820613878&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9065081&hvtargid=pla-1724930842708&psc=1&hvocijid=12931435962820613878-B09BDX69VX-&hvexpln=0&gad_source=1) |
