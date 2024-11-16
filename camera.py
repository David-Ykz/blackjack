from inference_sdk import InferenceHTTPClient
import cv2
import round_manager

cap = cv2.VideoCapture(1)
imagePath = "captured_frame.png"



def analyzeImage(imagePath):
    image = cv2.imread(imagePath)
    CLIENT = InferenceHTTPClient(
        api_url="https://detect.roboflow.com",
        api_key=""
    )
    result = CLIENT.infer(image, model_id="playing-cards-ow27d/4")
    print(result)
    return result["predictions"]


dealerCards = []
playerCards = []
roundManager = round_manager.RoundManager()

while True:
    ret, frame = cap.read()
#    frame = frame[:, :frame.shape[1] // 2]
    cv2.imshow("Video Capture", frame)

    key = cv2.waitKey(1) & 0xFF

    # initialize dealer / player, deal dealer / player
    if key == ord('q') or key == ord('w') or key == ord('e') or key == ord('r'):
        cv2.imwrite(imagePath, frame)
        print(f"Frame saved as {imagePath}")

        predictions = analyzeImage(imagePath)

        cardMapping = {
            "A": 1,
            "2": 2,
            "3": 3,
            "4": 4,
            "5": 5,
            "6": 6,
            "7": 7,
            "8": 8,
            "9": 9,
            "10": 10,
            "J": 10,
            "Q": 10,
            "K": 10,
        }
        cards = []

        for prediction in predictions:
            card = cardMapping[prediction["class"][:-1]]
            if card not in cards:
                cards.append(card)

        if key == ord('q'):
            dealerCards = cards
        elif key == ord('w'):
            playerCards = cards
            if len(playerCards) == 1:
                playerCards += playerCards
            print("Betting amount: ")
            print(roundManager.initializeRound(dealerCards, playerCards))

            
        elif key == ord('e'):
            dealerCards += cards
            roundManager.dealCard(cards[0], False)
        elif key == ord('r'):
            playerCards += cards
            print("Decision: ")
            print(roundManager.dealCard(cards[0], True))
            
                
        print(dealerCards)
        print(playerCards)
        print("")

#             if prediction['y'] > 320:
#                 print("Lower")
#                 print(prediction["class"])
# #                dealerCards.append(prediction["class"])
#             else:
#                 print("Upper")
#                 print(prediction["class"])

        

    elif key == ord('x'):
        break

cap.release()
cv2.destroyAllWindows()


