import cv2


def main():
    cap = cv2.VideoCapture("./input.mov", cv2.CAP_ANY)

    w = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    h = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fourcc = cv2.VideoWriter_fourcc(*"XVID")
    video_writer = cv2.VideoWriter("./output.mov", fourcc, 25, (w, h))

    area_border = 10000
    ret, frame = cap.read()
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    while True:

        pr_frame = frame
        ret, cur_frame = cap.read()
        source = cur_frame.copy()
        if not ret:
            break

        # cur_frame = cv2.cvtColor(cur_frame, cv2.COLOR_BGR2GRAY)
        cur_frame = cv2.GaussianBlur(cv2.cvtColor(cur_frame, cv2.COLOR_BGR2GRAY), (11, 11), 0)
        dif = cv2.absdiff(cur_frame, pr_frame)
        (T, thresh) = cv2.threshold(dif, 20, 255, cv2.THRESH_BINARY)
        contours, hierarchy = cv2.findContours(
            thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE
        )

        for i in range(len(contours)):
            item = cv2.contourArea(contours[i])
            if item > area_border:
                video_writer.write(source)

        if cv2.waitKey(10) & 0xFF == 27:
            break

    video_writer.release()
    cap.release()
    
    cv2.destroyAllWindows()


main()
